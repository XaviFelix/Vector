#include <iostream>
#include <algorithm>

template <typename Object>
class Vector
{
    public:
        explicit Vector(int initSize = 0) : theSize{initSize},
            theCapacity{initSize + SPARE_CAPACITY}
        {
            objects = new Object[theCapacity];
        }

        //**************************************************************
        //This is a copy constructor
        //**************************************************************
        
        Vector(const Vector& rhs) : theSize{rhs.theSize},
            theCapacity{rhs.theCapacity}, objects{nullptr}
        {
            objects = new Object[theCapacity];
            for(int i = 0; i < theSize; ++i)
                objects[i] = rhs.objects[i];
        }

        
        //Operator overloading '='
        Vector & operator= (const Vector& rhs)
        {
            Vector copy = rhs;
            std::swap(*this, copy);
            return *this;
        }

        //This is the destructor
        ~Vector() {delete[] objects;}

        //**************************************************************
        //This is a move constructor using move semantics
        //**************************************************************
        Vector(Vector&& rhs) : theSize{rhs.theSize}, 
            theCapacity{rhs.theCapacity}, objects{rhs.objects}
        {
            rhs.objects = nullptr;
            rhs.theSize = 0;
            rhs.theCapacity = 0;
        }

        Vector & operator= (Vector && rhs)
        {
            std::swap(objects, rhs.objects);
            std::swap(theSize, rhs.theSize);
            std::swap(theCapacity, rhs.theCapacity);

            return *this;
        }
        //**************************************************************
        //**************************************************************

        void reSize(int newSize)
        {
            if(newSize > theCapacity)
                reserve(newSize * 2);

            theSize = newSize;
        }

        void reserve(int newCapacity)
        {
            //Throw a message?
            if(newCapacity < theSize)
                return;

            Object *newArray = new Object[newCapacity];

            //Has to be from i to theSize because that was how many elements
            //The previous array had, can't be newCapacity since that would not make sense
            for(int i = 0; i < theSize; ++i)
                newArray[i] = std::move(objects[i]);

            theCapacity = newCapacity;

            //After the new array was created it is now swapped back to the original array
            //named objects
            std::swap(objects, newArray);

            //We must finish off by deleting the created newArray since
            //This is NOT JAVA!
            delete[] newArray;
        }

        Object & operator[](int index) {return objects[index];} //This one is used for non-const objects
        const Object & operator[](int index) const {return objects[index];} //This one is used for const objects!

        bool isEmpty() const {return getSize() == 0;}
        int getSize() const {return theSize;}
        int getCapacity() const {return theCapacity;}

        void pushBack(const Object& item)
        {
            if(theSize == theCapacity)
                reserve(2 * theCapacity + 1);

            objects[theSize++] = item;
        }

        void pushBack(Object && item)
        {
            if(theSize == theCapacity)
                reserve(2 * theCapacity + 1);

            objects[theSize++] = std::move(item);
        }

        void popBack()
        {
            --theSize;
        }

        const Object& back() const {return objects[theSize - 1];}

        typedef Object *iterator;
        typedef const Object *constIterator;

        iterator begin()
        {
            return &objects[0];
        }

        const iterator begin() const
        {
            return &objects[0];
        }

        iterator end() {return &objects[getSize()];}
        const iterator end() const {return &objects[getSize()];}

        


    static const int SPARE_CAPACITY = 16;

    private:
        int theSize; //initialSize
        int theCapacity; // iewSiznitialSize + spare capacity
        Object *objects;
};


//This is a test program
auto main() -> int
{
    std::cout << "This is a test" << '\n';

    Vector<int> myVector;

    for(int i = 0; i < 10; ++i)
    {
        if(i == 2)
            myVector.pushBack(100);
        else
            myVector.pushBack(0);
    }
    Vector<int> myVector2 = std::move(myVector);

    std::cout << myVector2[2] << '\n';
    std::cout << "This is the size of my Vector: " <<  myVector2.getSize() << '\n';
    std::cout << "This is the capacity of my Vector: " << myVector2.getCapacity() << '\n';
}