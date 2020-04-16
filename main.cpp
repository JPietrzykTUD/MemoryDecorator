
#include <iostream>
#include <vector>

#include <memory/memory.h>

int main( ) {
   using namespace tuddbs;

//   void * data = malloc( 16 * sizeof( uint64_t ) );
//   void * data2 = malloc( 16 * sizeof( uint64_t ) );
   void * data = acquire( 16 * sizeof( uint64_t ), 64);
   void * data2 = acquire( 16 * sizeof( uint64_t ), 64);
   uint64_t * data_64 = ( uint64_t * ) data;
   uint64_t * data2_64 = ( uint64_t * ) data2;
   for( std::size_t i = 0; i < 20; ++i ) {
      data_64[ i ] = i;
      data2_64[ i ] = i+1;
   }
   for( std::size_t i = 0; i < 16; ++i ) {
      std::cerr << data_64[ i ] << "\n";
   }
   release( data2 );
   release( data );
//   free( data2 );
//   free( data );


   return 0;
}
