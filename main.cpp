#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <array>
#include "classes.h"
#include "TST.h"


//=====================
//==GLOBAL  VARIABLES==
//=====================
std::vector<job> jobQ;
std::vector<std::string> messageQ;
std::vector<metaPointer> metaPointStack;

//=============
//==CONSTANTS==
//=============
//Function spec segment
const std::array <std::string,1> funcCodes={"TST"};		// code of function
const int funcPtNum[]={1};					// Number of pointers function takes
const bool funcStNum[]={true};				// Does function take settings
std::array <std::vector<std::string>,1> funcStCod;				//setting codes
std::array <std::vector<std::string>,1> funcStTyp;				//setting types
								//   ^ change number if increasing num of commands (bad code i know)
void funcStCodFill(){
	funcStCod[0]={"TRLL"};
	funcStTyp[0]={"bool"};
	return;
}
	// List of acceptable setting called at startup. Bit of a fucky solution but i cba to fix it.

//function codes:
//[CMP] = Command processor
//[FCH] = function handler
//[INP] = Input handler
//[OUT] = Output handler
//[SCH] = Soundcard handler
//[STH] = Storage handler
//[TIC] = Tick handler
//[TST] = test function

//==GENERAL FUNCTIONS==

//==================
//==CORE FUNCTIONS==
//==================
bool inputHdlr(bool); //a false return denotes program exit
bool functionHdlr(bool, job);

//==CORE SUB-FUNCTIONS==
job commandProcess(bool debug,std::string command);

//==================
//==CALL FUNCTIONS==
//==================
//void test(bool, bool, int);

//========
//==MAIN==
//========
int main(){
	funcStCodFill();

bool debug = true;
		if(debug){std::cout << "[TIC] Startup\n";}
while(true){
int nJobs = jobQ.size();
			if(debug){std::cout << "[TIC] checking job queue\n";}
			if(debug){std::cout << "[TIC] " << nJobs << " jobs in queue\n";}
	if(nJobs>0){
		for (int i=0;i<nJobs;i++){
					if(debug){std::cout << "[TIC] Executing job "<< i << " of " << nJobs << "\n";}
			if(functionHdlr(debug, jobQ[i])){
				messageQ.push_back("MQ: [TIC] function handler executed sucessfully\n");
			}else{
				messageQ.push_back("MQ: [TIC] function handler failed\n");
			}
					if(debug){std::cout << "[TIC] Checking message queue\n";}
			int nMess = messageQ.size();
			if(nMess >0){
				for(int j=0;j<nMess;j++){
							if(debug){std::cout << "[TIC] Reading message "<< j << " of " << nMess << "\n";}
					std::cout << messageQ[j];
				}
			}
		}
	}
			if(debug){std::cout << "[TIC] Calling input handler\n";}
	if(inputHdlr(debug)){
			if(debug){std::cout << "[TIC] Returned true\n";}
	}else{
			if(debug){std::cout << "[TIC] Returned false, terminating program\n";}
			return 0;
	}
}
}

//==GENERAL FUNCTIONS==

//==================
//==CORE FUNCTIONS==
//==================

bool inputHdlr(bool debug){
			if(debug){std::cout << "[INP] Called\n[INP] Ready for input\n";}
	std::vector<std::string> commands;
	std::vector<job> internalJobQ;
	job tempJob;
	while(true){
		//get raw input string from user
		char chIn[256];
		std::string uInput;
		std::cout << "~";
		std::cin.getline (chIn,256);
		uInput=chIn;
		while(uInput[0]==' '){
			uInput.erase(0,1);
		}
		// this will be in format XXXXXXX~XXXXXXX~XX~XXXXX
		// where X is part of a command. This means the character ~ is a special character not to be used in commands
		// this next section separates out the commands into a list of commands
		// the variable start keeps track of the start of the last command.
		int start = 0;
		int prevCommand = commands.size();//keeps track of the last command thats been categorised
		for (int i =0; i< uInput.length();i++){
			if (uInput[i]=='~' or i==uInput.length()-1){
				if (i==uInput.length()-1){i++;}
				commands.push_back(uInput.substr(start,i-start));
						if(debug){std::cout << "[INP] Command " << commands.size()-1 << ": " << uInput.substr(start,i-start) << "\n";}
				start=i+1;
			}
		}
		//this next code separates the commands into their six categories
		//these categories are based on the first letter of the command
		//X = exit program		S = sumbit jobs to queue
		//C = pull data from buffer to ouput handler
		//D = submit job for storage handler
		//V = submit job for soundcard handler
		//F = submit job for function handler
				if(debug){std::cout << "[INP] Categorizing commands\n";}
		for (int i =prevCommand; i< commands.size(); i++){
			switch (commands[i][0]){
				case 'x': case 'X':
							if(debug){std::cout << "[INP] Termination request, inputHdlr returning false to tick handler\n";}
					return false;	
				break;
				case 's': case 'S':
							if(debug){std::cout << "[INP] Submitting " <<commands.size()-1<<" jobs\n";}
					jobQ = internalJobQ;
							if(debug){std::cout << "[INP] Submitted jobs# sucessfully, returning to tick handler\n";}
					return true;
				break;
				case 'c': case 'C':
					if(i==0){
									if(debug){std::cout << "[INP] Command "  << i << " Data pull called as initial command. Will be direct executed\n";}
						//!process command here
						//!call output handler directly here
						}else{
									if(debug){std::cout << "[INP] Command "  << i << " Data pull called as job. Will be added to job queue.\n";}
							tempJob = commandProcess(debug, commands[i]);
							if (tempJob.funcID==""){
										if(debug){std::cout << "[INP] handed null job, discarding\n";}
							}else{
								internalJobQ.push_back(tempJob);
							}
						}
				break;
				case 'f': case 'F':
				case 'd': case 'D':
				case 'v': case 'V':
							if(debug){std::cout << "[INP] Command "  << i << " Job request\n";}
					tempJob = commandProcess(debug, commands[i]);
					if (tempJob.funcID==""){
								if(debug){std::cout << "[INP] handed null job, discarding\n";}
					}else{
						internalJobQ.push_back(tempJob);
					}
				break;
				default:
				std::cout << "[INP] ERROR in command "  << i << " (error code 001): no valid commmands start with '" << commands[i][0] << "' ignoring command\n";
				break;
			}
		}
	}
}

bool functionHdlr(bool debug, job mJob){
	//calls all the call functions, essentially one large switch statement
	if(mJob.funcID=="TST"){
		bool trllVal;
		if(mJob.settVal.size()==0){
			trllVal=true;
		}else if(mJob.settVal[0]=="True" or mJob.settVal[0]=="true" or mJob.settVal[0]=="TRUE" or mJob.settVal[0]=="1" or mJob.settVal[0]==""){
			trllVal=true;
		}else if(mJob.settVal[0]=="False" or mJob.settVal[0]=="false" or mJob.settVal[0]=="FALSE" or mJob.settVal[0]=="0"){
			trllVal=false;
		}else{
			messageQ.push_back("MQ: [FCH] TST setting TRLL handed bad value, taking as true\n");
			trllVal=true;
		}
		if(mJob.pStack.size()==0){
					if(debug){std::cout << "[FCH] ERROR code 001.6 handed no pointers\n";}
			return false;
		}
		test(debug,trllVal, mJob.pStack[0]);
		return true;
	}else{
				std::cout << "[FTH] Error code 001.5 passed job has invalid tag. Returning false";
		return false;
	}
	return false;
}
//======================
//==CORE SUB-FUNCTIONS==
//======================

job commandProcess(bool debug,std::string command){
	job mJob;
	int funcNum; //which function from the list of constants
	//Structure of command 
	// XYYYnum,num,.../str,str,...
	//where X=type initialiser (see earlier, C/D/V/F)
	//YYY = 3char function code (only if X = F)
	//num,num... = list of pointerStack codes
	//str,str,... = list of setting codes (only barred chars are '~' and ',')
	//Settings handled as XXXXVal
	//where XXXX=4char setting code and Val=value of code 
	//if no value is given then a boolean true is assumed as value.
			if(debug){std::cout << "[CMP] Initialised, processing command into job\n";}
for(int i=command.size()-1;i>=0;i--){
	if(command[i]==' '){command.erase(i,1);}
}
	switch(command[0]){
		case 'd': case 'D':
			command.erase(0,1);
			mJob.funcID = "STH";
					if(debug){std::cout << "[CMP] Storage handler command called, job tagged STH\n";}
		break;
		case 'v': case 'V':
			command.erase(0,1);
			mJob.funcID = "SCH";
					if(debug){std::cout << "[CMP] Sound card handler command called, job tagged SCH\n";}
		break;
		case 'c': case 'C':
			command.erase(0,1);
			mJob.funcID = "OUT";
					if(debug){std::cout << "[CMP] Output handler command called, job tagged OUT\n";}
		break;
		case 'f': case 'F':
			command.erase(0,1);
			if((int)command[0]>=(int)'a'){command[0]=(char)((int)command[0] -32);}
			if((int)command[1]>=(int)'a'){command[1]=(char)((int)command[1] -32);}
			if((int)command[2]>=(int)'a'){command[2]=(char)((int)command[2] -32);}
			mJob.funcID = {command[0],command[1],command[2]};
					if(debug){std::cout << "[CMP] Function handler command called, job tagged ";  std::cout << mJob.funcID << "\n";}
			for(int i =0;i<funcCodes.size()+1;i++){
				if(i==funcCodes.size()){
					std::cout << "[CMP] ERROR code 001.2 Function tag "<< mJob.funcID <<" not valid, returning null job to INP\n";
					job null;
					return null;
				}
				if(mJob.funcID==funcCodes[i]){
							if(debug){std::cout << "[CMP] Function tag recognised as valid\n";}
					funcNum = i;
					i=funcCodes.size()+2; // yes i know this is a little janky
				}
			}
			command.erase(0,3);
		break;
		default:
			std::cout<<"[CMP] ERROR code 001.1 handed bad command, returning null job to IMP\n";
			job null;
			return null;
		break;
	}
	if(command.size()==0){
		if(funcPtNum[funcNum]!=0){
			std::cout << "[CMP] insufficient number of pointers for function tag " << mJob.funcID << ". " << mJob.funcID << " takes " << funcPtNum[funcNum] << " pointers\n";
			std::cout<<"[CMP] ERROR code 001.3 returning null job to IMP\n";
			job null;
			return null;
		}
				if(debug){std::cout << "[CMP] no pointers or settings provided, returning tag only job\n";}
		return mJob;
	}
	//separate the pointers from the settings
	std::string strPoint;
	std::string strSett;
	for(int i =0; i<command.length(); i++){
		if (command[i]=='/'){
			strPoint = command.substr(0,i);
			strSett = command.substr(i+1);
			i=command.length();
		} else if(i == command.length()-1){
			strPoint = command;
		}
	}
	//processing strPoint.
	std::vector<int> pointers;
	int start = 0;		//keeps track of where the last comma was
	std::string temp;	//holds string of pointer num temporarily 
	for(int i =0; i<strPoint.size(); i++){
		
		if(strPoint[i]==','){
			temp = strPoint.substr(start,i-start);
			start = i+1;
			pointers.push_back(stoi(temp));
		}else if(i==strPoint.size()-1){
			temp = strPoint.substr(start,i-start+1);
			pointers.push_back(stoi(temp));
		}
	}
	//check number of pointers is correct
	if(pointers.size()!=funcPtNum[funcNum]){
		std::cout << "[CMP] incorrect number of pointers for function tag " << mJob.funcID 
		<< ". " << mJob.funcID << " takes " << funcPtNum[funcNum] << " pointers, "<< pointers.size() << " pointers given\n ";
		std::cout<<"[CMP] ERROR code 001.3 returning null job to IMP\n";
		job null;
		return null;
	}
	//outputs pointer
	if(debug){
		std::cout<< "[CMP] pointers are: " ;
		for(int i =0;i<pointers.size();i++){
			std::cout << pointers [i] << " , ";
		}
		std::cout << "\n";
	}
	
	mJob.pStack=pointers;
	
	//if there are no settings
	if(strSett.size()==0){
				if(debug){std::cout << "[CMP] No settings given \n[CMP] Command processed into job, returning to IMP\n";}
		return mJob;
	}
	
	//processing strSett
	std::vector<std::string> tempVec;		//holds separated setting commands
	std::vector<std::string> settTags;		//holds setting tags
	std::vector<std::string> settVals;			//holds setting values
	//separate into tempVec;
	start=0;
	for(int i =0; i<strSett.size(); i++){
		
		if(strSett[i]==','){
			tempVec.push_back(strSett.substr(start,i-start));
			start = i+1;
		}else if(i==strSett.size()-1){
			tempVec.push_back(strSett.substr(start,i-start+1));
		}
	}
	//capitalise the first four letters of setting tag
	for(int i=0; i<tempVec.size();i++){
		for(int j=0;j<4;j++){
			if((int)tempVec[i][j]>=(int)'a'){tempVec[i][j]=(char)((int)tempVec[i][j]-32);}
		}
	}
	//check and assign setting tags
	for(int i=0; i<tempVec.size();i++){
		for(int j=0; j<funcStCod.size();j++){
			if(tempVec[i].substr(0,4)==funcStCod[funcNum][j]){
				settTags.push_back(funcStCod[funcNum][j]);
				settVals.push_back(tempVec[i].substr(4,tempVec.size()-4));
				j=tempVec.size();
			}
			if(j==tempVec.size()-1){
				std::cout<< "[CMP] Error code 001.4. " << tempVec[i].substr(0,4) << " is not a valid setting code for " << mJob.funcID << " type function\n";
				std::cout<<"[CMP] Returning null job to IMP\n";
				job null;
				return null;
			}
		}
	}
	if(debug){std::cout<< "[CMP] Setting codes all valid\n";}
	
	if(debug){
		std::cout<< "[CMP] settings are: " ;
		for(int i =0;i<settTags.size();i++){
			std::cout << settTags[i] << ":" << settVals[i] << ", ";
		}
		std::cout << "\n";
	}
	
	if(debug){std::cout << "[CMP] Command processed into job, returning to IMP\n";}
	return mJob;
}

