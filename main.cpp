
#include <iostream>
#include <vector>

#include <memory/memory.h>

int main( ) {
   using namespace tuddbs;
   std::cout << "Hello, World!" << std::endl;

   std::vector< void * > vec;

   for( std::size_t i = 0; i < 3; ++i ) {
      vec.push_back( acquire( 16 * sizeof( uint64_t ), 64, i, "st iteration" ) );

   }
   std::size_t c = 0;
   for( auto a : vec ) {
      uint64_t * b = ( uint64_t * ) a;
      for( std::size_t i = 0; i < 17; ++i ) {
         b[ i ] = 0x808080;
      }
      inspect( a );
   }
   for( auto a : vec ) {
      release( a );
   }


   return 0;
}
