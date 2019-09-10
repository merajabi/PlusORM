#include <iostream>
#include "inc/orm.h"
#include "person.h"
#include "book.h"

using namespace PlusORM;

int main(int argc, char **argv) {

	{
		ORM* model = ORM::GetInstance(); // Get an instance of ORM 
		Person::Initialize();      // Initialize Person (sync with db)
		Book::Initialize();

		Person john ("John", "Black", 33);                 // Create new Person object
		model->Insert(john);                             // Insert object in DB

		Book b1 ("John Biography", john.GetId(), 2017);                 // Create new Person object
		model->Insert(b1);                             // Insert object in DB

		std::cout << model->Count(Person::GetTableName()) << std::endl;        // Number of Person table row in DB
		std::cout << model->Count(Book::GetTableName()) << std::endl;        // Number of Person table row in DB

		john.SetAge(34);                                // change the Person object
		model->Update(john);                             // Update the DB

		b1.SetYear(2018);                                // change the Person object
		model->Update(b1);                             // Update the DB

	}
	{
		ORM* model = ORM::GetInstance(); // Get an instance of ORM 
		Person::Initialize();      // Initialize Person (sync with db)
		Book::Initialize();

		model->Search(Person::GetTableName(),"*","1 = 0; SELECT 'Person' as Person, a.*, 'Book' as Book, b.* FROM Person as a JOIN Book as b ON a.id = b.Author;");					 // Search persons with Age > 30
		const std::list<ObjectMap*> list=model->GetResultList();
		for(std::list<ObjectMap*>::const_iterator it=list.begin(); it != list.end(); ++it){
			std::cout << Person::GetId(*it) << "\t" << Person::GetFirst(*it) << "\t" << Person::GetLast(*it) << "\t" << Person::GetAge (*it) << std::endl;
			std::cout << Book::GetId(*it) << "\t" << Book::GetName(*it) << "\t" << Book::GetAuthor(*it) << "\t" << Book::GetYear (*it) << std::endl;
		}

		model->Drop(Person::GetTableName());                                   // Drom Person table in DB, if you wish
		model->Drop(Book::GetTableName());                                   // Drom Person table in DB, if you wish
	}

	ORM::RemoveInstance();                   // Remove ORM instance, just at the end of program.
	std::cout << "Finish" << std::endl;
	return 0;
}
