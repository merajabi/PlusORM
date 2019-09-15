#include <iostream>
#include <vector>
#include "ModernCPP/include/ModernCPP.h"
#include "inc/orm.h"
#include "person.h"

using namespace PlusORM;

const int numTry=5;
const int numThread=4;
const long incAge=20;
mutex m;

void inc1(Person& man){
	ORM* model = ORM::GetInstance();		// Get an instance of ORM 
	std::string query="Update Person set age=age+1 where Id="+toString(man.GetId());
	for(long i=0;i<incAge;i++){
			lock_guard<mutex> lk(m);
			model->Query(query);		// Sqlite compiled in theadsafe mode SQLITE_THREADSAFE=1
			model->Sync(man);
	}
}

void inc2(Person& man){
	ORM* model = ORM::GetInstance();		// Get an instance of ORM 
	for(long i=0;i<incAge;i++){				// it works like this so when we just write to database keeping object safe is enough
			lock_guard<mutex> lk(m);
			man.SetAge(man.GetAge()+1);		// change the Person object
			model->Update(man);
	}
}

int main() {
	Person::Initialize();      // Initialize Person (sync with db)
	Person john ("John", "Black", 0);

	{
		ORM* model = ORM::GetInstance(); // Get an instance of ORM 
		model->Insert(john);                             // Insert object in DB
		ORM::RemoveInstance(); 
	}
	for (int i=0;i<numTry;i++) {
		std::vector<thread> tv;
		for(int j=0;j<numThread/2;j++){
			tv.push_back(thread(inc1,ref(john)));
			tv.push_back(thread(inc2,ref(john)));
		}
		for(size_t j=0;j<tv.size();j++){
			tv[j].join();
		}
		ORM::RemoveInstance();
	}
	std::cout << john.GetId() << "\t" << john.GetFirst() << "\t" << john.GetLast() << "\t" << john.GetAge () << std::endl;
	{
		ORM* model = ORM::GetInstance(); // Get an instance of ORM 
		model->Search(Person::GetTableName(),"*","Id = "+toString(john.GetId()));
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

