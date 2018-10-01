# PlusORM
## A Simple C++ Object Relational MAP
### Features
- simple and intuitive interface
- support for C++98 to be compatible with old embedded toolchains
- automatic code generation

### Notice
The project is still naive it currently support operations on distinct tables no join implemented yet.
As Donald Knuth stated premature optimization is the root of all evil, so nothing is optimized yet.

## Tutorial & Usage 
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
```

## Building 
PlusORM has standard Makefile in the root folder of the project optimized for use in embedded environment by the help of shell script
### X86 Environment
extract the project in your local directory and issue following commands

```sh
# first cd to project directory
cd PlusORM
# then build the library with this command
./build.sh
# test the library with this command
./build.sh test
# issue this one to clean the projcet
./build.sh clean
```
### Cross Compilation
The Makefile of the project is witten in the way to facilitate the cross compilation for embedded environment. To cross compile the project use the x86.mk file in the root folder as a template and rename it to something meaningful, insert the toolchain path and any required library and any architecture specific flags required, then issue following commands

```sh
# assume we have created arm-linux-gnueabihf.mk
./build.sh arm-linux-gnueabihf.mk
# issue this one to clean the projcet
./build.sh clean
```
the PlusORM library along with sqlite and utils will be created as dynamic link library (.so) for your target platform, and you can add them to your project.

