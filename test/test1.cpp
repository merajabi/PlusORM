#include <iostream>
#include "inc/orm.h"
#include "person.h"

using namespace PlusORM;

int main(int argc, char **argv) {

	{
		ORM<Person>* model = ORM<Person>::GetInstance(); // Get an instance of ORM 
		Person::Initialize(model->MaxPrimaryKey());      // Initialize Person (sync with db)

		Person john ("John", "Doe", 33);                 // Create new Person object

		model->Insert(john);                             // Insert object in DB

		std::cout << model->Count() << std::endl;        // Number of Person table row in DB

		john.SetAge(34);                                // change the Person object
		model->Update(john);                             // Update the DB
	}
	{
		ORM<Person>* model = ORM<Person>::GetInstance(); // Get an instance of ORM 
		Person::Initialize(model->MaxPrimaryKey());      // Initialize Person (sync with db)

		model->Search("*","Age > 30");					 // Search persons with Age > 30
		const std::list<Person*> list=model->GetResultList();
		for(std::list<Person*>::const_iterator it=list.begin(); it != list.end(); ++it){
			Person* ptr = *it;
			std::cout << ptr->GetId() << "\t" << ptr->GetFirst() << "\t" << ptr->GetLast() << "\t" << ptr->GetAge () << std::endl;
		}
		//model->Drop();                                   // Drom Person table in DB, if you wish
	}

	ORM<Person>::RemoveInstance();                   // Remove ORM instance, just at the end of program.
	std::cout << "Finish" << std::endl;
	return 0;
}
