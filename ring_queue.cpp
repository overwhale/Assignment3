// Description: An incomplete implementation of iterators for a
//              RingQueue class.
//
// Notes: The project DOES compile but there is no meaningful 
//              output
//
// Your job: To complete this set of classes so that the output
//              produced by 'main' (below), matches the sample
//              file provided at the end of this file.


#include <iostream>

// Forward declaration
template <typename ItemType, int MAX_SIZE>
class RingQueue;



template <typename ItemType, int MAX_SIZE>
class RingQueue{
    
    // Nested Forward declaration of RingQueue<ItemType,MAX_SIZE>::iterator
    // This is needed if one plans to turn this home-made iterator into
    // one of the special categories of iterators (e.g., input, output,
    // forward, etc.).
    public: 
        class iterator;
        class const_iterator;


    // Aliases. 
    typedef ItemType* pointer;
    typedef ItemType& reference;



    // Definition of RingQueue<ItemType,MAX_SIZE>::iterator
    public:
        class iterator{
            private:
                // A link to the parent container 
                RingQueue* parent;

                // The position within the RingQueue is determined
                // by how far ahead we are from the begining of the
                // queue.
                int offset;

            private:  // Private constructor???
                iterator(RingQueue* _parent, int _offset = 0 )
                  : parent(_parent), offset(_offset) { }

                
            // It is quite common for Containers and their iterators
            // to be friends. After all, they should work closely together.
            friend class RingQueue<ItemType,MAX_SIZE>;


            public:
                reference operator*() {
					return parent->buffer[(parent->begin_index + offset) % MAX_SIZE ];
                }

                iterator& operator++(){
                    this->offset++;
                    return *this;
                }

                iterator operator++( int unused ){
                    iterator result(*this);
                    this->offset++;
                    return result;
                }

                bool operator==( const iterator& rhs ) const {
                    return this->offset == rhs.offset;
                }

                bool operator!=( const iterator& rhs ) const {
                    return this->offset != rhs.offset ||
                        this->parent != rhs.parent;
                }

        };



       
        class const_iterator{
            private:
                RingQueue* parent;
                int offset;

            private:
                // Only RingQueue objects can create const_iterators...
                const_iterator(RingQueue* _parent, int _offset = 0)
                    : parent(_parent), offset(_offset) { }

            public:
                // ... however, const_iterators can be 'copied'.
                const_iterator( const const_iterator& a){
                    this->offset = a.offset;
                    this->parent = a.parent;
                }

            friend class RingQueue<ItemType,MAX_SIZE>;
        };
        



    // Friendship goes both ways here.
    friend class iterator;
    friend class const_iterator; 



    private:
        // A fixed-size static array with constant capacity that represents 
        // the RingQueue
        ItemType buffer[MAX_SIZE];

        // The starting index. It changes according to a very 
        // specific set of rules (below).
        int begin_index;

        // The actual size of the RingQueue. Not to be confused with
        // its capacity. 
        int ring_size;



        // A helper function that computes the index of 'the end'
        // of the RingQueue
        int end_index() const {
            return (begin_index + ring_size) % MAX_SIZE;
        }



    public: 
        // Constructor
        RingQueue() : begin_index(0), ring_size(0) { }
		

        // Accessors. Note: 'back()' is not considered part of the array.
        ItemType front() const { 
            if ( ring_size == 0 ) std::cerr<< "Warning: Empty ring!\n" ;

            return buffer[begin_index];
        }
        ItemType back() const {  
            if ( ring_size == 0 ) std::cerr<< "Warning: Empty ring!\n" ;

			if ( end_index() > 0 )
				return buffer[end_index() - 1]; 
	    
            return buffer[MAX_SIZE];
        }



        // Mutators
        void push_back( const ItemType& value ){
			*end() = value;
			// If the queue is full 
			if (ring_size == MAX_SIZE) {
				if (begin_index < MAX_SIZE) {
					begin_index++;
				}else {
					begin_index = 0;
				}
			}else{								// The queue is not full yet
				ring_size++;
			}
            return;
        }

        void pop_front(){
            ring_size--;
			if (begin_index < MAX_SIZE)
				begin_index++;
			else
				begin_index = 0;
            return;
        }

        // Functions that return iterators
        iterator begin() { 
            return iterator(this, 0); 
        }
        iterator end() {
            return iterator(this, ring_size);
        }

        // Miscellaneous functions
        size_t size() const {
            return ring_size;
        }

        // Debugging functions
        void dump_queue() const {
            std::cout << "Raw queue...\n";
            for ( size_t i = 0 ; i < MAX_SIZE ; ++i )
                std::cout << "Val: " << buffer[i] << ", at: " << buffer+i << '\n';
            std::cout << '\n';
            return;
        }

};

int main(){
    RingQueue<int,7> rq;
    rq.dump_queue();

    for ( int i = 0 ; i < 8 ; ++i )
        rq.push_back(i+1);

    rq.dump_queue();
    rq.pop_front();

    std::cout << "Queue via size: \n";

    // RingQueue<int,7>::iterator it = rq.begin() ; 
    auto it = rq.begin() ; 
    for ( size_t i = 0 ; i < rq.size() ; ++i ) {
        std::cout << "Value: " << *it << ", address: " << &(*it) << '\n';
        ++it;
    }
    std::cout << '\n';

    

    // Uncomment the block below only when you have a working 
    // implementation of RingQueue<ItemType,int>::end(). 
    // If the implementation is not correct, it might result in 
    // an infinite loop.
 
    std::cout << "Queue via iterators: \n";
    for ( auto it = rq.begin() ; it != rq.end() ; ++it ) {
        std::cout << "Value: " << *it << ", address: " << &(*it) << '\n';
    }
    std::cout << '\n';




    rq.dump_queue();
	system("PAUSE");
    return 0;
}



/** 
    +++++++++++++++++++++++++++++++++++++++++++++++
    My output:
    Raw queue...
    Val: -858993460, at: 00000084771DF698
    Val: -858993460, at: 00000084771DF69C
    Val: -858993460, at: 00000084771DF6A0
    Val: -858993460, at: 00000084771DF6A4
    Val: -858993460, at: 00000084771DF6A8
    Val: -858993460, at: 00000084771DF6AC
    Val: -858993460, at: 00000084771DF6B0

    Raw queue...
    Val: 8, at: 00000084771DF698
    Val: 2, at: 00000084771DF69C
    Val: 3, at: 00000084771DF6A0
    Val: 4, at: 00000084771DF6A4
    Val: 5, at: 00000084771DF6A8
    Val: 6, at: 00000084771DF6AC
    Val: 7, at: 00000084771DF6B0

    Queue via size:
    Value: 3, address: 00000084771DF6A0
    Value: 4, address: 00000084771DF6A4
    Value: 5, address: 00000084771DF6A8
    Value: 6, address: 00000084771DF6AC
    Value: 7, address: 00000084771DF6B0
    Value: 8, address: 00000084771DF698

    Queue via iterators:
    Value: 3, address: 00000084771DF6A0
    Value: 4, address: 00000084771DF6A4
    Value: 5, address: 00000084771DF6A8
    Value: 6, address: 00000084771DF6AC
    Value: 7, address: 00000084771DF6B0
    Value: 8, address: 00000084771DF698

    Raw queue...
    Val: 8, at: 00000084771DF698
    Val: 2, at: 00000084771DF69C
    Val: 3, at: 00000084771DF6A0
    Val: 4, at: 00000084771DF6A4
    Val: 5, at: 00000084771DF6A8
    Val: 6, at: 00000084771DF6AC
    Val: 7, at: 00000084771DF6B0
 */
