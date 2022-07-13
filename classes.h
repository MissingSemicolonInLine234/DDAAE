class job{
	public:
		std::string funcID;
		std::vector<std::string> settCode;
		std::vector<std::string> settVal;
		std::vector<int> pStack;
};
class jobReturn{
	public:
		std::vector<std::string> messages;
		bool success;
};
class metaPointer{
	public:
		int pStart;					//start of data in buffer
		int pStop;					//end of data in buffer
		std::string description;	//description for user
		std::string funcHistCod;	//all functions + settings that have operated on the pointer in terms of the codes of the functions
		std::string funcHistDesc;	//all functions + settings that have operated on the pointer but user readable
		std::string format;			//format for displaying data
};