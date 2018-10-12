struct zeroLengthException: public exception{
	const char * what() const throw(){
		return "Read zero length data. Maybe connection was closed";
	}
};