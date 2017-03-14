#include <iostream>
#include <stdlib.h>
#include <vector>
#include "smart_ptr.h"

using namespace std;

class Test{
public:
    int counter_;
    int number;
    Test() noexcept: number(0), counter_(0){}
    Test(int num) noexcept: number(num), counter_(0){}
    ~Test(){
        ++counter_;
    }
};

class Test_Enable: public Enable_Shared_From_This<Test_Enable>{
public:
    int number;
    Test_Enable() noexcept: number(0){}
    Test_Enable(int n) noexcept: number(n){}
    ~Test_Enable(){}
    void test(int n){
        My_Shared_Ptr<Test_Enable> sh_ptr = Shared_From_This();
        sh_ptr.reset(new Test_Enable(n));
    }

};

class My_Deleter{
public:
    int ch;
    My_Deleter(): ch(0){};
    My_Deleter(int n): ch(n){};
    void operator()(Test* ptr){
        delete ptr;
    }
};



void test_unique_ptr()
{

    cout << "-------Tests for Unique Pointer-------\n";

    Test* Obj1 = new Test(1);
    My_Unique_Ptr<Test> ptr1(Obj1);
    cout << "My_Unique_Ptr<T>(T* Obj), operator *\n";
    if ((*ptr1).number == 1){
        cout << "OK\n";
    }
    else{
        cout << "* Failed";
        exit(1);
    }

    cout << "operator -> \n";
    if (ptr1->number == 1){
        cout << "OK\n";
    }
    else{
        cout << "-> Failed\n";
        exit(1);
    }


    Test* Obj_to_reset = new Test(2);
    My_Unique_Ptr<Test> ptr_to_reset(new Test(1));
    cout << "reset()\n";
    ptr_to_reset.reset(Obj_to_reset);
    if (ptr_to_reset->number == 2){
        cout << "OK\n";
    }
    else{
        cout << "Reset failed\n";
        exit(1);
    }


    cout << "release()\n";
    Test* released = ptr_to_reset.release();
    if ((released->number == 2) && (ptr_to_reset.get() == nullptr)){
        cout << "OK\n";
    }
    else{
        cout << "Release failed\n";
        exit(1);
    }



    cout << "swap()\n";
    Test* to_swap1 = new Test(3);
    Test* to_swap2 = new Test(4);
    My_Deleter del1(1);
    My_Deleter del2(2);

    My_Unique_Ptr<Test, My_Deleter> ptr_to_swap1(to_swap1, del1);
    My_Unique_Ptr<Test, My_Deleter> ptr_to_swap2(to_swap2, del2);

    ptr_to_swap1.My_Unique_Ptr::swap(ptr_to_swap2);
    if ((ptr_to_swap1.get()->number == 4) && (ptr_to_swap2.get()->number == 3) && (ptr_to_swap1.get_deleter().ch == 2) && (ptr_to_swap2.get_deleter().ch == 1)){
        cout << "OK\n";
    }
    else{
        cout << "Swap failed\n";
        exit(1);
    }


    cout<<"Compare\n";
    ptr_to_swap1.My_Unique_Ptr::swap(ptr_to_swap2);
    if ((ptr_to_swap1 < ptr_to_swap2) != (to_swap1 < to_swap2)){
        cout <<"Operator < failed\n";
        exit(1);
    }
    if ((ptr_to_swap1 > ptr_to_swap2) != (to_swap1 > to_swap2)){
        cout <<"Operator > failed\n";
        exit(1);
    }
    if ((ptr_to_swap1 <= ptr_to_swap2) != (to_swap1 <= to_swap2)){
        cout <<"Operator <= failed\n";
        exit(1);
    }
    if ((ptr_to_swap1 >= ptr_to_swap2) != (to_swap1 >= to_swap2)){
        cout <<"Operator >= failed\n";
        exit(1);
    }
    if ((ptr_to_swap1 == ptr_to_swap2) != (to_swap1 == to_swap2)){
        cout <<"Operator == failed\n";
        exit(1);
    }
    if ((ptr_to_swap1 != ptr_to_swap2) != (to_swap1 != to_swap2)){
        cout <<"Operator != failed\n";
        exit(1);
    }
    cout << "OK\n";

    cout << "operator =(My_Unique_Ptr &&other)\n";
    Test* Obj2 = new Test(5);
    Test* Obj3 = new Test(6);
    My_Unique_Ptr<Test, My_Deleter> ptr2(Obj2, My_Deleter(2));
    My_Unique_Ptr<Test, My_Deleter> ptr3(Obj3, My_Deleter(3));

    ptr2 = move(ptr3);
    if (((ptr2.get())->number == Obj3->number) && ((ptr2.get_deleter()).ch == 3)){
        cout <<"OK\n";
    }
    else{
        cout << "failed\n";
        exit(1);
    }


    cout << "My_Unique_Ptr(My_Unique_Ptr &&other)\n";
    Test* Obj5 = new Test(7);
    My_Unique_Ptr<Test, My_Deleter> ptr5(Obj5, My_Deleter(5));
    My_Unique_Ptr<Test, My_Deleter> ptr4 = move(ptr5);
    if (((ptr4.get())->number == 7) && ((ptr4.get_deleter()).ch == 5)){
        cout << "OK\n";
    }
    else{
        cout << "failed\n";
        exit(1);
    }

    cout << "Bool\n";
    if (!ptr_to_reset && ptr2){
        cout << "OK\n";
    }
    else{
        cout << "Failed\n";
        exit(1);
    }

    cout << "Make_unique\n";
    My_Unique_Ptr<Test, My_Deleter> ptr6 = My_Unique_Ptr<Test, My_Deleter>::make_unique(Test(10));
    if (ptr6->number == 10){
        cout << "OK\n";
    }
    else{
        cout << "Failed\n";
        exit(1);
    }



    cout << "Vector\n";
    vector<My_Unique_Ptr<Test, My_Deleter> >a;
    a.push_back(My_Unique_Ptr<Test, My_Deleter>(new Test(10), My_Deleter(10)));
    if (a[0]->number == 10){
        cout << "OK\n";
    }
    else{
        cout << "Failed\n";
        exit(1);
    }
}


void test_shared_ptr(){

    cout << "-------Tests for Shared Pointer-------\n";

    Test* Obj1 = new Test(1);
    My_Shared_Ptr<Test> ptr1(Obj1);
    cout << "My_Shared_Ptr<T>(T* Obj), operator *\n";
    if ((*ptr1).number == 1){
        cout << "OK\n";
    }
    else{
        cout << "* Failed";
        exit(1);
    }


    Test* Obj2 = new Test(2);
    My_Shared_Ptr<Test> ptr2(Obj2);
    cout << "Compare\n";
    bool f = true;
    if ((ptr1 > ptr2) != (Obj1 > Obj2)){
        cout << "> Failed\n";
        f = false;
    }
    if ((ptr1 >= ptr2) != (Obj1 >= Obj2)){
        cout << ">= Failed\n";
        f = false;
    }
    if ((ptr1 < ptr2) != (Obj1 < Obj2)){
        cout << "< Failed\n";
        f = false;
    }
    if ((ptr1 <= ptr2) != (Obj1 <= Obj2)){
        cout << "<= Failed\n";
        f = false;
    }
    if ((ptr1 == ptr2) != (Obj1 == Obj2)){
        cout << "== Failed\n";
        f = false;
    }
    if ((ptr1 != ptr2) != (Obj1 != Obj2)){
        cout << "< Failed\n";
        f = false;
    }
    if (f){
        cout << "OK\n";
    }
    else{
        cout << "Compare failed\n";
        exit(1);
    }


    Test* Obj_to_reset = new Test(2);
    My_Shared_Ptr<Test> ptr_to_reset(Obj_to_reset);
    cout << "reset()\n";
    ptr1.reset(Obj_to_reset);
    if (ptr1->number == 2){
        cout << "OK\n";
    }
    else{
        cout << "Reset failed\n";
        exit(1);
    }

    cout << "Create with My_deleter\n";

    Test* to_swap1 = new Test(3);
    My_Deleter del1(1);
    My_Shared_Ptr<Test> ptr_to_swap1(to_swap1, del1);
    if (ptr_to_swap1->number == 3){
        cout <<"OK\n";
    }
    else{
        cout << "Failed\n";
        exit(1);
    }

    cout << "swap()\n";
    Test* to_swap3 = new Test(4);
    My_Deleter del3(3);
    My_Shared_Ptr<Test> ptr_to_swap3(to_swap3, del3);

    ptr_to_swap1.My_Shared_Ptr::swap(ptr_to_swap3);
    if ((ptr_to_swap1->number == 4) && (ptr_to_swap3->number == 3)){
        cout << "OK\n";
    }
    else{
        cout << "Swap failed\n";
        exit(1);
    }

    cout << "Bool\n";
    ptr2.reset(nullptr);
    if(!ptr2 && ptr1){
        cout << "OK\n";
    }
    else{
        cout << "Failed\n";
        exit(1);
    }


    cout << "Copy constructor\n";
    My_Shared_Ptr<Test> ptr3(new Test(5));
    My_Shared_Ptr<Test> ptr4 = ptr3;
    if(ptr3->number == ptr4->number){
        cout << "OK\n";
    }
    else{
        cout << "Failed\n";
        exit(1);
    }

    cout << "Operator=\n";
    ptr4 = ptr3;
    if(ptr3->number == ptr4->number){
        cout << "OK\n";
    }
    else{
        cout << "Failed similar\n";
        exit(1);
    }
    My_Shared_Ptr<Test> ptr5(new Test(6));
    ptr4 = ptr5;
    if(ptr4->number == 6){
        cout << "OK\n";
    }
    else{
        cout << "Failed different\n";
        exit(1);
    }

    cout << "Make Shared\n";
    My_Shared_Ptr<Test> ptr6 = My_Shared_Ptr<Test>::Make_Shared(Test(1));
    if (ptr6->number == 1){
        cout << "OK\n";
    }
    else{
        cout << "Failed\n";
        exit(1);
    }

    cout << "Create from My_Unique_Ptr\n";
    My_Deleter del(1);
    My_Unique_Ptr<Test, My_Deleter> u_ptr(new Test(1), del);
    My_Shared_Ptr<Test> sh_ptr = move(u_ptr);
    if (sh_ptr->number == 1){
        cout << "OK\n";
    }
    else{
        cout << "Failed\n";
        exit(1);
    }


}

void test_weak_ptr(){
    cout << "-------Tests for Weak Pointer-------\n";

    cout << "lock()\n";
    My_Shared_Ptr<Test> sh_p1(new Test(1));
    My_Weak_Ptr<Test> ptr1 = sh_p1;
    My_Weak_Ptr<Test> ptr2 = ptr1;
    My_Shared_Ptr<Test> sh_p2 = ptr2.lock();
    if (sh_p1->number == sh_p2->number){
        cout << "OK\n";
    }
    else{
        cout << "Failed\n";
        exit(1);
    }

    cout << "Operator =(My_Shared_Ptr &other)\n";

    My_Shared_Ptr<Test> sh_p3(new Test(2));
    ptr2 = sh_p3;
    sh_p1 = ptr2.lock();
    if (sh_p1->number == 2){
        cout << "OK\n";
    }
    else{
        cout << "Failed\n";
        exit(1);
    }

    cout << "Expired()\n";
    My_Weak_Ptr<Test> ptr3;
    My_Shared_Ptr<Test> sh_p4;
    ptr3 = sh_p4;
    My_Weak_Ptr<Test> ptr4 = ptr3;
    if ( ptr2.expired()){
        cout << "Failed on not nullptr\n";
        exit(1);
    }
    if(!ptr3.expired()){
        cout << "Failed on nullptr\n";
        exit(1);
    }
    if(!ptr3.expired()){
        cout << "Failed on empty constructor\n";
        exit(1);
    }

    if(!ptr4.expired()){
        cout << "Failed on operator = from nullptr\n";
        exit(1);
    }
    cout << "OK\n";

}

void test_enable_shared_from_this(){
    cout << "-------Tests for Enable Shared From This-------\n";
    Test_Enable* Obj = new Test_Enable(1);
    My_Shared_Ptr<Test_Enable> ptr1(Obj);
    My_Shared_Ptr<Test_Enable> ptr2 = Obj->Shared_From_This();
    ptr2->test(2);
    if ((ptr2->number == 2) && (ptr1->number == 2)){
        cout << "OK\n";
    }
    else{
        cout << "Failed\n";
        exit(1);
    }
}

int main(){
    test_unique_ptr();
    test_shared_ptr();
    test_weak_ptr();
    test_enable_shared_from_this();
    return 0;
}
