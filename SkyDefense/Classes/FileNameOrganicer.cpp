#include "FileNameOrganicer.h"


FileNameOrganicer* FileNameOrganicer::instance = nullptr;

FileNameOrganicer* FileNameOrganicer::getInstance(){
	if (instance == nullptr){
		instance = new FileNameOrganicer();
	}
	return instance;
}