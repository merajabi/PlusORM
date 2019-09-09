#include "gtest/gtest.h"
//#include "notest.h"

#include "inc/orm.h"
#include "person.h"

namespace {

	using namespace PlusORM;

	class ORMTest 
					: public ::testing::Test  // uncomment for gtest
	{
	protected:
		PlusORM::ORM* model;
//	public:									// comment for gtest
		void SetUp() {
			model=PlusORM::ORM::GetInstance();
			//ASSERT_NE(model,(PlusORM::ORM<PlusORM::Person>*)nullptr);
			Person::Initialize();
			EXPECT_EQ(model->Count(Person::GetTableName()),0ul);

			Person john ("John", "Doe", 33);
			Person jane ("Jane", "Doe", 32);
			Person joe  ("Joe", "Dirt", 30);

			// Make objects persistent and save their ids for later use.
			ASSERT_TRUE(model->Insert(john));
			ASSERT_TRUE(model->Insert(jane));
			ASSERT_TRUE(model->Insert(joe));

			EXPECT_EQ(model->Count(Person::GetTableName()),3ul);
		}

		void TearDown() {
			ASSERT_TRUE(model->Drop(Person::GetTableName()));
			PlusORM::ORM::RemoveInstance();
		}
	};									// uncomment for gtest

	TEST_F(ORMTest,PersonInsert) {

		Person bill  ("Bill", "Dirt", 34);
		ASSERT_TRUE(model->Insert(bill));
		ASSERT_FALSE(model->Insert(bill));
		EXPECT_EQ(model->Count(Person::GetTableName()),4ul);
	}

	TEST_F(ORMTest,PersonSearch1){
			
			ASSERT_TRUE(model->Search(Person::GetTableName(),"*","id == 1"));
			const std::list<ObjectMap*> list=model->GetResultList();
			EXPECT_EQ(list.size(),1ul);
			for(std::list<ObjectMap*>::const_iterator itl=list.begin(); itl != list.end(); itl++){
				Person ptr(*(*itl));
				EXPECT_EQ(ptr.GetId (),1ul);
				EXPECT_EQ(ptr.GetFirst(),"John");
				EXPECT_EQ(ptr.GetLast(), "Doe");
				EXPECT_EQ(ptr.GetAge (), 33);
			}
	}

	TEST_F(ORMTest,PersonSearch2){
			ASSERT_TRUE(model->Search(Person::GetTableName(),"*","Age > 30"));
			const std::list<ObjectMap*> list=model->GetResultList();
			EXPECT_EQ(list.size(),2ul);
			for(std::list<ObjectMap*>::const_iterator itl=list.begin(); itl != list.end(); itl++){
				Person ptr(*(*itl));
				if(itl==list.begin()){
					EXPECT_EQ(ptr.GetId (),1ul);
					EXPECT_EQ(ptr.GetFirst(),"John");
					EXPECT_EQ(ptr.GetLast(), "Doe");
					EXPECT_EQ(ptr.GetAge (), 33);
				}else{
					EXPECT_EQ(ptr.GetId (),2ul);
					EXPECT_EQ(ptr.GetFirst(),"Jane");
					EXPECT_EQ(ptr.GetLast(), "Doe");
					EXPECT_EQ(ptr.GetAge (), 32);
				}
			}
	}
	TEST_F(ORMTest,PersonSearch3){
			ASSERT_TRUE(model->Search(Person::GetTableName(),"*","First == \"Joe\" AND Last == \"Dirt\""));
			const std::list<ObjectMap*> list=model->GetResultList();
			EXPECT_EQ(list.size(),1ul);
			for(std::list<ObjectMap*>::const_iterator itl=list.begin(); itl != list.end(); itl++){
				Person ptr(*(*itl));
				EXPECT_EQ(ptr.GetId (),3ul);
				EXPECT_EQ(ptr.GetFirst(),"Joe");
				EXPECT_EQ(ptr.GetLast(), "Dirt");
				EXPECT_EQ(ptr.GetAge (), 30);
			}
	}
	TEST_F(ORMTest,PersonSearch4){
			ASSERT_TRUE(model->Search(Person::GetTableName(),"*", "1==1 ORDER BY `id` DESC LIMIT 1"));
			const std::list<ObjectMap*> list=model->GetResultList();
			EXPECT_EQ(list.size(),1ul);
			for(std::list<ObjectMap*>::const_iterator itl=list.begin(); itl != list.end(); itl++){
				Person ptr(*(*itl));
				EXPECT_EQ(ptr.GetId (),3ul);
				EXPECT_EQ(ptr.GetFirst(),"Joe");
				EXPECT_EQ(ptr.GetLast(), "Dirt");
				EXPECT_EQ(ptr.GetAge (), 30);
			}
	}
	TEST_F(ORMTest,PersonRemove){
			ASSERT_TRUE(model->Search(Person::GetTableName(),"*","Age > 30"));
			const std::list<ObjectMap*> list=model->GetResultList();
			EXPECT_EQ(list.size(),2ul);
			for(std::list<ObjectMap*>::const_iterator itl=list.begin(); itl != list.end(); itl++){
				Person ptr(*(*itl));
				EXPECT_TRUE(model->Remove(ptr));
			}
		EXPECT_EQ(model->Count(Person::GetTableName()),1ul);
	}
	TEST_F(ORMTest,PersonUpdate){
		{
			ASSERT_TRUE(model->Search(Person::GetTableName(),"*","id == 1"));
			const std::list<ObjectMap*> list=model->GetResultList();
			EXPECT_EQ(list.size(),1ul);
			if(list.size()){
				Person ptr ( *(*(list.begin())) );
				ptr.SetAge(40);
				EXPECT_TRUE(model->Update(ptr));
			}
		}
		{
			ASSERT_TRUE(model->Search(Person::GetTableName(),"*","id == 1"));
			const std::list<ObjectMap*> list=model->GetResultList();
			//list.pop_back();
			EXPECT_EQ(list.size(),1ul);
			if(list.size()){
				Person ptr ( *(*(list.begin())) );
				EXPECT_EQ(ptr.GetAge (), 40);
			}
		}
	}
	TEST_F(ORMTest,PersonConst){
		const Person petter("Petter", "Black", 54);
		ASSERT_TRUE(model->Insert(petter));
		EXPECT_EQ(model->Count(Person::GetTableName()),4ul);
		//petter.SetAge(56);
		ASSERT_TRUE(model->Update(petter));
	}

//	};									// comment for gtest

};
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);	// uncomment for gtest
	return RUN_ALL_TESTS();					// uncomment for gtest

	// comment ALL for gtest
	/*
	ORMTest obj;
	obj.SetUp();
	obj.PersonInsert();
	obj.TearDown();

	obj.SetUp();
	obj.PersonSearch1();
	obj.TearDown();

	obj.SetUp();
	obj.PersonSearch2();
	obj.TearDown();

	obj.SetUp();
	obj.PersonSearch3();
	obj.TearDown();

	obj.SetUp();
	obj.PersonSearch4();
	obj.TearDown();

	obj.SetUp();
	obj.PersonRemove();
	obj.TearDown();

	obj.SetUp();
	obj.PersonUpdate();
	obj.TearDown();

	obj.SetUp();
	obj.PersonConst();
	obj.TearDown();
	return 0;
	*/
}

