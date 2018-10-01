#ifndef _ResultSet_H_
#define _ResultSet_H_


namespace PlusORM {

typedef std::map<std::string,std::string> Row;

//typedef std::list<std::map<std::string,std::string>*> Table;

class ResultSet : public std::list<std::map<std::string,std::string>*> {
public:
	~ResultSet(){
		while (!empty()) {
			Row *r=front();
			pop_front();
			r->clear();
			delete r;
		}
	}
};

};

#endif //_ResultSet_H_

