#include <iostream>
#include "inc/orm.h"
#include "person.h"

using namespace PlusORM;

int main(int argc, char **argv) {

	ORM<Person>* model;

	model=ORM<Person>::GetInstance();
	Person::Initialize(model->MaxPrimaryKey());

	Person john ("John", "Doe", 33);

	model->Insert(john);

	std::cout << model->Count() << std::endl;

	model->Drop();

	ORM<Person>::RemoveInstance();
	std::cout << "Finish" << std::endl;
	return 0;
}

