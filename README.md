# PlusORM
## A Simple C++ Object Relational MAP

The following code show how simple we can interact with database using this ORM


### Example Table Schema
assume we have table named `Person` with following sql schema

```sql
CREATE TABLE IF NOT EXISTS `Person` (
`id` INTEGER PRIMARY KEY NOT NULL,
`First` VARCHAR(128) NOT NULL,
`Last` VARCHAR(128) NOT NULL,
`Age` INTEGER
);

```
A simple Perl script in (script/GendbInterface.pl) will create a C++ class interface for this table automatically

```sh
script/GendbInterface.pl < person.sql > person.h
```

### Example Model Object
we will include this C++ class interface in our project and use it as follow

```c++

#include <iostream>
#include "inc/orm.h"
#include "person.h"

using namespace PlusORM;

int main(int argc, char **argv) {

	ORM<Person>* model = ORM<Person>::GetInstance(); // Get an instance of ORM 
	Person::Initialize(model->MaxPrimaryKey());      // Initialize Person (sync with db)

	Person john ("John", "Doe", 33);                 // Create new Person object

	model->Insert(john);                             // Insert object in DB

	std::cout << model->Count() << std::endl;        // Number of Person table row in DB

  john->SetAge(34);                                // change the Person object
	model->Update(john);                             // Update the DB

	model->Drop();                                   // Drom Person table in DB, if you wish

	ORM<Person>::RemoveInstance();                   // Remove ORM instance, just at the end of program.
	std::cout << "Finish" << std::endl;
	return 0;
}

