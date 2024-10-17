#ifndef RBUFFER_H
#define RBUFFER_H
#include <iostream>

template <class T>
class rBuffer {
    T *buffer_p;
    T *read_p; 
    //starts at the first index of the buffer, reads something and then
    //increments itself.
    T *write_p;

    int size;
    bool full;
    bool empty;
public:

    rBuffer(int=8);
    rBuffer(const rBuffer<T> &);
    ~rBuffer();
    T read();
    void write(T);
    bool resize(int);
    void clear();
    void print() const;
    template <class A, class B>
    friend rBuffer<A> joinBuffers(const rBuffer<A> &, const rBuffer<B> &);
};

template <class T>
void rBuffer<T>::print() const{
    std::cout << " | ";
    for (int i = 0; i < size; i++) {
        if (buffer_p + i == read_p) 
            std::cout << "*";
        if (buffer_p + i == write_p)
            std::cout << "+";
        std::cout << buffer_p[i] << " | ";
    }
    std::cout << "\n";
}

// Constructor
template <class T>
rBuffer<T>::rBuffer(int sizeIn) {  
    // Initialize buffer
    if (sizeIn < 1) {
        sizeIn = 8;
    }
    this->size = sizeIn;
    this->empty = true; 
    this->full = false; 

    this->buffer_p = new T[this->size] {}; //using {} as initializers

    // Initialize read and write pointers
    this->read_p = this->buffer_p;
    this->write_p = this->buffer_p;
}

// Copy constructor
template<typename T>
rBuffer<T>::rBuffer(const rBuffer<T> &otherBuffer){
    // Initialize buffer
    this->size = otherBuffer.size; 
    this->buffer_p = new T [size] {}; //allocates new memory
    this->empty = otherBuffer.empty; // copies empty status
    full = otherBuffer.full; // copy the full and empty flags

    //relative to array
    //current location of person being helped in line
    // T already created once function is called
    int read_offset = (otherBuffer.read_p - otherBuffer.buffer_p); 

    //otherBuffer.buffer_p front of line
    int write_offset = (otherBuffer.write_p - otherBuffer.buffer_p);


    //can't point to the front 
    this->read_p = this->buffer_p + read_offset;  // points to the same place
    this->write_p = this->buffer_p + write_offset; //last location
    
    //put contents of otherBuffer to buffer_p
    for(int i = 0; i < write_offset ; ++i){
        buffer_p[i] = otherBuffer.buffer_p[i];  // copies the contents of otherBuffer to buffer_p
    }
}

template<typename T>
rBuffer<T>::~rBuffer(){ 
    delete[] buffer_p;

    /*removes dangling pointers*/
    this->buffer_p = nullptr;
    this->read_p = nullptr;
    this->write_p = nullptr;
}

template<typename T>
T rBuffer<T>::read(){
    if(this->empty)
        throw "Cannot read from empty buffer!";
    
    if(this->read_p == this->write_p){ 

        if(this->buffer_p == this->read_p){
            return *(this->buffer_p + this->size - 1);
        } 
        //dereference; return last read value 
        //changes ptr to data type it is pointing to
        return *(this->read_p - 1);
    }

    //point at diff locations in buffer
    T value = *read_p ; 
    
    //increments pointer //only buffer_p is array, 
    read_p++;
   
    //wraps around to beginning 
    // read_p pointing past the end of the array
    if(read_p == (buffer_p + size))
       read_p = buffer_p;  //then lead back to beginning of the array
    return value;  
}


template<typename T>
void rBuffer<T>::write(T temp){
    *this->write_p = temp; // changes value store in the write_ptr
    this->write_p++; //increments

    if(this->write_p == (this->buffer_p + this->size )){
        this->write_p = buffer_p; //move back to beginning
        this->full = true;
    }
    
    this->empty = false; 
}


template<class T>
bool rBuffer<T>::resize(int newSize) {
    // the number of elements written to buffer_p 
    int count = this->full ? this->size : this->write_p - this->buffer_p;

    if (newSize < count) { // if newSize is smaller than the number of elements written to buffer_p
        return false;
    }

    // store old buffer data
    T* old = this->buffer_p;

    int readOffset = this->read_p - this->buffer_p; // offset from beginning of buffer_p

    this->buffer_p = new T[newSize] {}; // allocate new buffer
    this->size = newSize; // update size
    this->read_p = this->buffer_p + readOffset; // update read_p
    this->write_p = this->buffer_p + count; // update write_p

    // set the old data into the new buffer
    for (int i = 0; i < count; ++i) { // copy old data into new buffer
        this->buffer_p[i] = old[i];
    }

    // if after resizing the write pointer is now off the end,
    // resized to the point where count == newSize,
    // so the buffer is now full
    if (this->write_p == this->buffer_p + this->size) { // if write_p is at the end of the buffer
        this->write_p = this->buffer_p; // move write_p back to the beginning
        this->full = true; 
    } else {
    // otherwise, expand the buffer, so if it was full 
    //before it's definitely not now
        this->full = false;
    }
    delete[] old;
    // successful resize
    return true;
}

template<typename T>
void rBuffer<T>::clear(){
    T* old = this->buffer_p;
    this->empty = true;
    this->full = false;

    //allocating memory for a new buffer 
    this->buffer_p = new T[this->size] {}; 

    // need to set the uninitialized memory to 0
    for (int i = 0; i < this->size; ++i) {
        this->buffer_p[i] = 0;
    }

    this->read_p = this->buffer_p; // set read_p to the beginning of the buffer
    this->write_p = this->buffer_p; // set write_p to the beginning of the buffer

    delete[] old;
}

/* joins the buffers */
template <class A, class B>
rBuffer<A> joinBuffers(const rBuffer<A> &lhs, const rBuffer<B> &rhs){
    rBuffer<A> joinedBuffer(lhs.size + rhs.size);

    // used size_t for better portability between machines
    // size_t is an unsigned integer type that is guaranteed to be the same size as a pointer
    size_t lhsCount = lhs.full ? lhs.size : (lhs.write_p - lhs.buffer_p);
    size_t rhsCount = rhs.full ? rhs.size : (rhs.write_p - rhs.buffer_p);

    for(size_t i = 0; i < lhsCount ; ++i){
        joinedBuffer.write(lhs.buffer_p[i]);
    }

    for(size_t i = 0; i < rhsCount; ++i){
        joinedBuffer.write(rhs.buffer_p[i]);
    }

    return joinedBuffer;    
}
#endif
