#include <iostream>
#include "inc/orm.h"
#include "person.h"

using namespace PlusORM;

int main(int argc, char **argv) {

	{
		ORM* model = ORM::GetInstance(); // Get an instance of ORM 
		Person::Initialize();      // Initialize Person (sync with db)

		Person john ("John", "Doe", 33);                 // Create new Person object

		model->Insert(john);                             // Insert object in DB

		std::cout << model->Count(Person::GetTableName()) << std::endl;        // Number of Person table row in DB

		john.SetAge(34);                                // change the Person object
		model->Update(john);                             // Update the DB
	}
	{
		ORM* model = ORM::GetInstance(); // Get an instance of ORM 
		Person::Initialize();      // Initialize Person (sync with db)

		Person Jenni ("Jenni", "White", 23);                 // Create new Person object
		model->Insert(Jenni);                             // Insert object in DB
		std::cout << model->Count(Person::GetTableName()) << std::endl;        // Number of Person table row in DB

		model->Search(Person::GetTableName(),"*","Age > 20");					 // Search persons with Age > 30
		const std::list<ObjectMap*> list=model->GetResultList();
		for(std::list<ObjectMap*>::const_iterator it=list.begin(); it != list.end(); ++it){
			Person ptr(*(*it));
			std::cout << ptr.GetId() << "\t" << ptr.GetFirst() << "\t" << ptr.GetLast() << "\t" << ptr.GetAge () << std::endl;

			std::cout << Person::GetId(*(*it)) << "\t" << Person::GetFirst(*(*it)) << "\t" << Person::GetLast(*(*it)) << "\t" << Person::GetAge (*(*it)) << std::endl;
		}

		model->Drop(Person::GetTableName());                                   // Drom Person table in DB, if you wish
	}

	ORM::RemoveInstance();                   // Remove ORM instance, just at the end of program.
	std::cout << "Finish" << std::endl;
	return 0;
}
