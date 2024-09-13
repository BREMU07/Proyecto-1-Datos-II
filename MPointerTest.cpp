#include "gtest/gtest.h"
#include "MPointer.h"

// Test Fixture Class for MPointerGC
class MPointerGCTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Start the Garbage Collector before each test
        MPointerGC::getInstance().startGC(1);
    }

    void TearDown() override {
        // Stop the Garbage Collector after each test
        MPointerGC::getInstance().stopGC();
    }
};

// Test if Garbage Collector starts and stops correctly
TEST_F(MPointerGCTest, TestStartStopGC) {
    MPointerGC& gc = MPointerGC::getInstance();
    EXPECT_TRUE(gc.isRunning()); // After SetUp, GC should be running
    gc.stopGC();
    EXPECT_FALSE(gc.isRunning()); // After stopGC, GC should not be running
}

// Test memory allocation in MPointerGC
TEST_F(MPointerGCTest, TestMemoryAllocation) {
    MPointerGC& gc = MPointerGC::getInstance();
    
    int index = gc.allocate(sizeof(int));
    EXPECT_NE(index, -1); // Allocation should give a valid index
}

// Test memory deallocation in MPointerGC
TEST_F(MPointerGCTest, TestMemoryDeallocation) {
    MPointerGC& gc = MPointerGC::getInstance();
    
    int index = gc.allocate(sizeof(int));
    gc.removeReference(index); // Removing reference should deallocate the memory
    EXPECT_THROW(gc.getMemory(index), std::runtime_error); // Trying to access deallocated memory should throw
}

// Test MPointer creation and dereferencing
TEST_F(MPointerGCTest, TestMPointerNew) {
    MPointer<int> ptr = MPointer<int>::New();
    *ptr = 42;
    EXPECT_EQ(*ptr, 42); // Test dereferencing and value assignment
}

// Test MPointer assignment and reference counting
TEST_F(MPointerGCTest, TestMPointerAssignment) {
    MPointer<int> ptr1 = MPointer<int>::New();
    *ptr1 = 10;

    MPointer<int> ptr2;
    ptr2 = ptr1; // Test assignment operator
    EXPECT_EQ(*ptr2, 10); // ptr2 should now point to the same memory as ptr1
}

// Test that dereferencing a null pointer throws an exception
TEST_F(MPointerGCTest, TestDereferencingNullPointer) {
    MPointer<int> ptr;
    EXPECT_THROW(*ptr, std::runtime_error); // Dereferencing a null pointer should throw an exception
}

TYPED_TEST(MPointerTest,Shouldvoid)
{
	 ;

	EXPECT_TRUE(2 == 2);
	EXPECT_FALSE(2 == 1);
	EXPECT_EQUALS(2, 2);

}
