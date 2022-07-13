
//output handler has three return types
//1) Buffer data
//	Takes list of start - stop locations in the buffer
//	Takes format code 
//2) Pointer data
//	Takes list of pointer numbers
//	Takes boolean values of what metadata to display
//3) Pointer and associated buffer data
//	Takes list of pointer numbers

//This is determined with bool pOrB where true = pointer data and false = buffer data. 
//whether or not to display associated buffer data is indicated inside mData display

//metadata to display is 
// 	0=also display associated buffer data
//	1=location of data in buffer
//	2=data discription
//	3=data function history description
//	4=data function history codes
//	5=stored format code

jobReturn outHdlr(bool debug, bool pOrB, std::vector<int> pStackLoc, std::vector<std::array<int,2>> buffLoc, std::string formatCode, std::array<bool,6> mDataDisplay){
			if(debug){std::cout << "[OUT] called\n";}
	jobReturn jRet;
	if(pOrB){
		for(int i =0; i < pStackLoc.size(); i++){
			std::cout<< "[OUT]==============================\n[OUT] Pointer " << i << " of " << pStackLoc.size+1 << " (Pointer " <<pStackLoc[i] << "in stack):\n";
			if(mDataDisplay[0]){
				jRet.messages.push_back("[OUT] Error attempt to access pointer buffer data, not yet a coded feature\n");
			}
			if(mDataDisplay[1]){
				std::cout << "[OUT] Location of pointed data in buffer: " << metaPointStack[pStackLoc[i]].pStart << " to " << metaPointStack[pStackLoc[i]].pStop << ".\n";
			}
			if(mDataDisplay[2]){
				std::cout << "[OUT] User made description{\n" << metaPointStack[pStackLoc[i]].description << "\n}";
			}
			if(mDataDisplay[3]){
				std::cout << "[OUT] Function history on data by definitions{\n" << metaPointStack[pStackLoc[i]].funcHistDesc<< "\n}";
			}
			if(mDataDisplay[4]){
				std::cout << "[OUT] Function history on data by codes{\n" << metaPointStack[pStackLoc[i]].funcHistCod<< "\n}";
			}
			if(mDataDisplay[5]){
				std::cout << "[OUT] Display format code" << metaPointStack[pStackLoc[i]].format << ". \n";
			}
		}
		jRet.success = true;
				if(debug){std::cout << "[OUT] Returning";}
		return jRet;
	}else{
		jRet.messages.push_back("[OUT] Error attempt to access raw buffer data, not yet a coded feature\n");
		jRet.success=false;
		return jRet;
	}
}
