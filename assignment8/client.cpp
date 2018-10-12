#include "tcpClient.cpp"

string protocol = "http";
int port = 80;
string domain = "";
string URI = "";
string ext = "";

string delim = "|=>|";
string cache_storage_file = "/tmp/cache/cache.list";

string payload, output,url_hash;

map<string,string> cache_map;

void update_cache_list(string hash,string file_location){
	cache_map[hash] = file_location;

	try{
		ofstream cache_list_file;
		cache_list_file.open(cache_storage_file.c_str(),ofstream::app);

		string temp = hash + " " + delim + " " + file_location + "\n";
		cache_list_file.write(temp.c_str(),temp.length());
		cache_list_file.close();
	}
	catch(exception& ex){
		fprintf(stderr, "Error while updating cache_list. Check if file or corresponding directory exists '%s'\n",&cache_storage_file[0]);
		exit(1);
	}
}

bool file_exists (const string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}

void load_cache(){
	if(!file_exists(cache_storage_file)){
		printf("Cache is empty\n");
		return;
	}

	ifstream cache_list_file;
	cache_list_file.open(cache_storage_file.c_str());

	string temp,hash,cached_file;
	
	while(getline(cache_list_file,temp)){
		hash = temp.substr(0,temp.find(delim) - 1);
		cached_file = temp.substr(temp.find(delim)+1+delim.length());

		printf("[*] %s : %s \n",&hash[0],&cached_file[0]);

		cache_map[hash] = cached_file;
	}

	cache_list_file.close();
	printf("Cache loaded\n");
}

bool check_cache(const string& hash) {
    return cache_map.find(hash) != cache_map.end();
}


void parse_url(string URL){
	if(URL.substr(0,7) != "http://"){
		fprintf(stderr,"Provided protocol not supported. Provided: %s\n",&URL.substr(0,7)[0]);
		exit(1);
	}

	URL = URL.substr(7);

	string::size_type n_colon,n_slash;

	n_colon = URL.find(':');
	n_slash = URL.find('/');

	if(n_slash == string::npos && n_colon == string::npos){ // http://xyz.com
		port = 80;
		URI = "/";
	}
	else if(n_slash == string::npos && n_colon != string::npos){ // http://xyz.com:596
		port = stoi(URL.substr(n_colon+1));
		URI = "/";
	}
	else if(n_slash != string::npos && n_colon == string::npos){ // http://xyz.com/dfsfda
		port = 80;
		URI = URL.substr(n_slash);
	}
	else{ // http://xyz.com:7856/dfsfda
		port = stoi(URL.substr(n_colon+1,n_slash-n_colon));
		URI = URL.substr(n_slash);
	}

	string::size_type min_n = (n_slash<n_colon)?n_slash:n_colon;

	if(min_n == string::npos){
		domain = URL;
	}
	else{
		domain = URL.substr(0,min_n);
	}

	printf("Protocol: %s\n",&protocol[0]);
	printf("Domain: %s\n",&domain[0]);
	printf("Port: %d\n",port);
	printf("URI: %s\n\n",&URI[0]);

	return;
}

void handle_file(){
	char* filename = (char *)cache_map[url_hash].c_str();

	printf("Handling file: %s\n",&filename[0]);

	char* const argv[] = {(char*)"/usr/bin/xdg-open", filename ,(char*)NULL};
	
	pid_t pid = fork();

	if(pid == 0){ // child
		execv("/usr/bin/xdg-open", argv);
	}
}

void cache_file(string body){
	string file_to_cache_in = "/tmp/cache/cache_"+url_hash+"."+ext;

	try{
		printf("Caching to: %s\n",&file_to_cache_in[0]);

		ofstream cacheFile;
		cacheFile.open (file_to_cache_in.c_str(), ofstream::binary);
		cacheFile.write(body.c_str(),body.length());
		cacheFile.close();
	}
	catch(exception& ex){
		fprintf(stderr, "Error while writing cache file. Check if corresponding directory exists '%s'\n","/tmp/cache/");
		exit(1);
	}

	update_cache_list(url_hash,file_to_cache_in);
}

string parse_raw_ext(string raw_ext){
	if(raw_ext == "text/html"){
		return "html";
	}
	else if(raw_ext == "image/jpeg"){
		return "jpg";
	}
	else if(raw_ext == "image/png"){
		return "png";
	}
	else if(raw_ext == "application/pdf"){
		return "pdf";
	}
	else if(raw_ext == "image/gif"){
		return "gif";
	}
	else{
		printf("Unkown extension type. Using HTML extension\n");
		return "html";
	}
}

void update_extension(string headers){
	string needle = "Content-Type: ";// Content-Type: text/html;
	string::size_type content_type_start = headers.find(needle);
	
	if(content_type_start == string::npos){
		printf("Unable to find Content-Type header in respose. Using HTML extension\n");
		ext = "html";
		return;
	}

	string::size_type semicolon_index = headers.find(';',content_type_start);
	if(content_type_start == string::npos){
		printf("Unkown response type. Using HTML extension\n");
		ext = "html";
		return;
	}

	string ext_raw = headers.substr(content_type_start+needle.size(),semicolon_index - (content_type_start+needle.size()));

	ext = parse_raw_ext(ext_raw);
}

void handler_function(TCPsocketHandler* socketObj){
	socketObj->sendData(payload);
	output = socketObj->recvData();

	string ok_str = "HTTP/1.1 200 OK";
	if(output.substr(0,ok_str.length()) == ok_str){
		printf("Response code 200 from server\n\n");
	}
	else{
		printf("Response code is not 200. Skipping file caching\n\n");
		printf("%s\n",&output[0]);
		return;
	}

	string headers = output.substr(0,output.find("\r\n\r\n"));
	printf("Response headers:\n%s\n\n",&headers[0]);

	string body = output.substr(output.find("\r\n\r\n")+4);
	// printf("Response body:\n%s\n",&body[0]);
	
	update_extension(headers);

	// cout<<body.length()<<endl;
	cache_file(body);

	handle_file();

	return;
}


int main(int argc,char* argv[]){
	if(argc != 2){
		printf("Usage: %s <url-to-page>\n",argv[0]);
		exit(1);
	}

	load_cache();

	string URL(argv[1]);

	parse_url(URL);

	url_hash = protocol+"://"+domain+":"+to_string(port)+URI;
	url_hash = to_string(hash<string>{}(url_hash));

	printf("Generated URL hash: %s\n",&url_hash[0]);

	if(check_cache(url_hash)){
		printf("Found in cache\n");

		handle_file();

		return 0;
	}

	tcpClient client(domain,port);

	payload = "GET "+URI +" HTTP/1.1\r\n";
	payload += "Host: "+domain+"\r\n";
	payload += "Connection: close";
	payload += "\r\n\r\n";

	printf("\n*PAYLOAD*\n%s\n", &payload[0]);

	client.startTalking((TCPsocketHandlerCallbackType)&handler_function);
	
	printf("\nDone :)\n");
	return 0;
}