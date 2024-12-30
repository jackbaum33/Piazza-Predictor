#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"
using namespace std;


TEST(test_empty) {
    BinarySearchTree<int,less<int>> t1;
    ASSERT_TRUE(t1.empty());
    t1.insert(1);
    ASSERT_FALSE(t1.empty());
}

TEST(test_insert_once)
{
    BinarySearchTree<int,less<int>> t2;
    t2.insert(2);
    BinarySearchTree<int,less<int>>::Iterator i1=t2.begin();
    ASSERT_EQUAL(*i1,2);
}

TEST(test_insert_multiple)
{
    BinarySearchTree<int,less<int>> t3;
    for(int i=1; i<=10; i++) t3.insert(i);
    BinarySearchTree<int,less<int>>::Iterator i1=t3.begin();
    for(int i=1; i<=10; i++)
    {
        ASSERT_EQUAL(*i1,i);
        i1++;
    }
}

TEST(test_size)
{
    BinarySearchTree<int,less<int>> t4;
    ASSERT_EQUAL(t4.size(),0);
    t4.insert(8);
    ASSERT_EQUAL(t4.size(),1);
    t4.insert(7);
    ASSERT_EQUAL(t4.size(),2);
    t4.insert(9);
    ASSERT_EQUAL(t4.size(),3);

}

TEST(test_height)
{
    BinarySearchTree<int,less<int>> t5;
    ASSERT_EQUAL(t5.height(),0);
    t5.insert(6);
    ASSERT_EQUAL(t5.height(),1);
    t5.insert(7);
    ASSERT_EQUAL(t5.height(),2);
     t5.insert(5);
    ASSERT_EQUAL(t5.height(),2);

}

TEST(test_size_big)
{
    BinarySearchTree<int,less<int>> t6;
    for(int i=0; i<300; i++)
    {
        ASSERT_EQUAL(t6.size(),i);
        t6.insert(i);
        ASSERT_EQUAL(t6.size(),i+1);
    }
}

TEST(test_height_big)
{
    BinarySearchTree<int,less<int>> t7;
    for(int i=2; i<=1000; i++)
    {
        t7.insert(i);
        ASSERT_EQUAL(t7.height(),i-1);
    }
    t7.insert(1);
    ASSERT_EQUAL(t7.height(),999);
    t7.insert(1001);
    ASSERT_EQUAL(t7.height(),1000);
}

TEST(test_sorting_invariant)
{
     BinarySearchTree<int,less<int>> t8;
     for(int i=0; i<10; i++)
     {
        t8.insert(i);
        ASSERT_TRUE(t8.check_sorting_invariant());
     }
      BinarySearchTree<int,less<int>>::Iterator i=t8.begin();
      *i=4;
      ASSERT_FALSE(t8.check_sorting_invariant());
}

TEST(test_max_element_standard)
{
   BinarySearchTree<int,less<int>> t9;
   for(int i=0; i<=500; i++) t9.insert(i);
   ASSERT_EQUAL((*t9.max_element()),500); 
}

TEST(test_max_element_negatives)
{
    BinarySearchTree<int,less<int>> t10;
     for(int i=0; i<=500; i++) t10.insert(i*-1);
    ASSERT_EQUAL((*t10.max_element()),0);
    t10.insert(5);
    ASSERT_EQUAL((*t10.max_element()),5);
}

TEST(test_max_empty)
{
  BinarySearchTree<int,less<int>> t11;
  BinarySearchTree<int,less<int>>::Iterator i1=t11.end();
  ASSERT_EQUAL(t11.max_element(),i1);  
}

TEST(test_min_element_standard)
{
 BinarySearchTree<int,less<int>> t12;
 t12.insert(8);
 t12.insert(76);
 t12.insert(777);
 ASSERT_EQUAL((*t12.min_element()),8);
}

TEST(test_min_element_negatives)
{
    BinarySearchTree<int,less<int>> t13;
     for(int i=0; i<=500; i++) t13.insert(i*-1);
    ASSERT_EQUAL((*t13.min_element()),-500);
    t13.insert(-8000);
    ASSERT_EQUAL((*t13.min_element()),-8000);
}

TEST(test_min_empty)
{
  BinarySearchTree<int,less<int>> t14;
  BinarySearchTree<int,less<int>>::Iterator i1=t14.end();
  ASSERT_EQUAL(t14.min_element(),i1);  
}

TEST(test_min_greater_than_standard)
{
  BinarySearchTree<int,less<int>> t15;
  t15.insert(44);
  t15.insert(0);
  t15.insert(99);
  ASSERT_EQUAL((*t15.min_greater_than(1)),44);
  ASSERT_EQUAL((*t15.min_greater_than(45)),99);
  ASSERT_EQUAL((*t15.min_greater_than(-1)),0);
}

TEST(test_min_greater_than_empty)
{
   BinarySearchTree<int,less<int>> t16;
  BinarySearchTree<int,less<int>>::Iterator i1=t16.end();
  ASSERT_EQUAL(t16.min_greater_than(0),i1);
  ASSERT_EQUAL(t16.min_greater_than(1000),i1);
  ASSERT_EQUAL(t16.min_greater_than(-7),i1);

}

TEST(test_find)
{
BinarySearchTree<int,less<int>> t17;
for(int i=0; i<=500; i++) t17.insert(i);
for(int i=0; i<=500; i++)
{
    ASSERT_EQUAL((*t17.find(i)),i);
}
ASSERT_EQUAL(t17.find(800),t17.end());
}

TEST(test_iterator_plusplus)
{
BinarySearchTree<int,less<int>> t18;
t18.insert(8);
t18.insert(9);
t18.insert(10);
BinarySearchTree<int,less<int>>::Iterator i1;
i1=t18.begin();
ASSERT_EQUAL(*i1,8);
i1++;
ASSERT_EQUAL(*i1,9);
i1++;
ASSERT_EQUAL(*i1,10);
}

TEST(test_iterator_plusplus_secondary_behavior)
{
    BinarySearchTree<int,less<int>> t19;
    t19.insert(7);
    t19.insert(8);
    t19.insert(6);
    t19.insert(5);
    t19.insert(3);
    t19.insert(4);
    BinarySearchTree<int,less<int>>::Iterator i1;
    i1=t19.begin(); 
    ASSERT_EQUAL(*i1,3);
    i1++;
    ASSERT_EQUAL(*i1,4);
    i1++;
    ASSERT_EQUAL(*i1,5);
    i1++;
    ASSERT_EQUAL(*i1,6);
    i1++;
    ASSERT_EQUAL(*i1,7);
    i1++;
    ASSERT_EQUAL(*i1,8);
}

TEST(test_iterator_equals_not_equals)
{
   BinarySearchTree<int,less<int>> t20; 
   for(int i=0; i<10;i++) t20.insert(i);
   BinarySearchTree<int,less<int>>::Iterator i1=t20.begin();
   BinarySearchTree<int,less<int>>::Iterator i2=t20.begin();
   for(int i=0; i<10; i++)
   {
    ASSERT_TRUE(i1==i2);
    ++i1;
    ASSERT_FALSE(i1==i2);
    ASSERT_TRUE(i1!=i2);
    ++i2;
    ASSERT_TRUE(i1==i2);
    ASSERT_FALSE(i1!=i2);
   }

}

TEST(test_copy_nodes_assignment_constructor)
{
    BinarySearchTree<int,less<int>> t20;
    for(int i=0; i<20;i++) t20.insert(i);
    BinarySearchTree<int,less<int>> copyt20(t20);
    ASSERT_EQUAL(copyt20.size(),20);
    ASSERT_EQUAL(copyt20.height(),20);
    ASSERT_TRUE(t20.size()==copyt20.size());
    ASSERT_TRUE(t20.height()==copyt20.height());

}

TEST(test_copy_nodes_assignment)
{
    BinarySearchTree<int,less<int>> t21;
    t21.insert(5);
    t21.insert(11);
    t21.insert(233);
    t21.insert(37);

    BinarySearchTree<int> copyt21;
    copyt21.insert(6);
    copyt21.insert(2);
    copyt21.insert(41);
    ASSERT_NOT_EQUAL(t21.size(),copyt21.size());
    copyt21 = t21;
    ASSERT_EQUAL(t21.size(), copyt21.size());
    ASSERT_EQUAL(t21.height(), copyt21.height());
}


TEST(test_traverse_in_order)
{
    BinarySearchTree<int,less<int>> t22;
    t22.insert(2);
    t22.insert(3);
    t22.insert(4);
    t22.insert(5);
    ostringstream treeInorder;
    t22.traverse_inorder(treeInorder);
    cout<<treeInorder.str()<<endl;
    ASSERT_TRUE(treeInorder.str()=="2 3 4 5 ");
}

TEST(test_traverse_preorder)
{
    BinarySearchTree<int,less<int>> t23;
    t23.insert(567);
    t23.insert(11);
    t23.insert(23);
    t23.insert(777);
    ostringstream treePreorder;
    t23.traverse_preorder(treePreorder);
    ASSERT_TRUE(treePreorder.str() == "567 11 23 777 ");
}

// TEST(fun_to_string)
// {
//     BinarySearchTree<int,less<int>> t;
//     t.insert(8);
//     t.insert(7);
//     t.insert(6);
//     t.insert(4);
//     t.insert(5);
//     t.insert(3);
//     t.insert(2);
//     t.insert(1);
//     t.insert(11);
//     t.insert(10);
//     t.insert(9);
//     cout<<t.to_string();

// }

TEST_MAIN()
