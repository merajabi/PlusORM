#include <iostream>
#include <vector>
#include "ModernCPP/include/ModernCPP.h"
#include "inc/orm.h"
#include "person.h"

using namespace PlusORM;


const int numTry=2;
const int numThread=2;
const long incAge=10;
mutex m;

void inc(Person& man){
		ORM* model = ORM::GetInstance();		// Get an instance of ORM 
		std::string query="Update Person set age=age+1 where Id="+toString(man.GetId());
		for(long i=0;i<incAge;i++){
			model->Query(query);		// Sqlite compiled in theadsafe mode SQLITE_THREADSAFE=1
		}
}

int main() {
	Person::Initialize();						// Initialize Person (sync with db)
	Person john ("John", "Black", 0);

	{
		ORM* model = ORM::GetInstance();		// Get an instance of ORM 
		model->Insert(john);					// Insert object in DB
		//ORM::RemoveInstance(); 
	}
	for (int i=0;i<numTry;i++) {
		std::vector<thread> tv;
		for(int j=0;j<numThread;j++){
			tv.push_back(thread(inc,ref(john)));
		}
		for(size_t j=0;j<tv.size();j++){
			tv[j].join();
		}
		//ORM::RemoveInstance();
	}
	{
		ORM* model = ORM::GetInstance();		// Get an instance of ORM 
		model->Search(Person::GetTableName(),"*");
		const std::list<ObjectMap*> list=model->GetResultList();
		if(list.size()){
			std::list<ObjectMap*>::const_iterator it=list.begin();
			std::cout << Person::GetId(*it) << "\t" << Person::GetFirst(*it) << "\t" << Person::GetLast(*it) << "\t" << Person::GetAge (*it) << std::endl;
		}
		model->Drop(Person::GetTableName()); 
		ORM::RemoveInstance(); 
	}

	return 0;
}

