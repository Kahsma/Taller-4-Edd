#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// -------------------------------------------------------------------------
void QuadTree(
  unsigned short** image,
  unsigned int w0, unsigned int h0,
  unsigned int w1, unsigned int h1,
  std::ostream& out
  )
{
  unsigned short init_val = image[ h0 ][ w0 ];
  bool same_val = true;
  for( unsigned int w = w0; w < w1 && same_val; ++w )
    for( unsigned int h = h0; h < h1 && same_val; ++h )
      same_val &= ( image[ h ][ w ] == init_val );

  if( !same_val )
  {
    unsigned int hw = ( w1 + w0 ) >> 1;
    unsigned int hh = ( h1 + h0 ) >> 1;
    out << "2";
    QuadTree( image, w0, h0, hw, hh, out );
    QuadTree( image, hw, h0, w1, hh, out );
    QuadTree( image, hw, hh, w1, h1, out );
    QuadTree( image, w0, hh, hw, h1, out );
  }
  else
    out << init_val;
}

// -------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
  if( argc < 2 )
  {
    std::cerr
      << "Usage: "
      << argv[ 0 ] << " input_pgm > output_qt"
      << std::endl;
    return( 1 );

  } // fi

  // Read PGM image as a buffer
  std::ifstream input_pgm_file(
    argv[ 1 ], std::ios::in | std::ios::binary | std::ios::ate
    );
  if( !( input_pgm_file.is_open( ) ) )
  {
    std::cerr
      << "Error opening file: \"" << argv[ 1 ] << "\""
      << std::endl;
    return( 1 );

  } // fi
  std::streampos size = input_pgm_file.tellg( );
  char* buffer = new char[ size ];
  input_pgm_file.seekg( 0, std::ios::beg );
  input_pgm_file.read( buffer, size );
  input_pgm_file.close( );

  // Read PGM data
  std::stringstream input_pgm_stream( buffer );
  char line[ 1024 ];
  input_pgm_stream.getline( line, 1024 ); // Magic number
  input_pgm_stream.getline( line, 1024 ); // Comment

  unsigned short W, H;
  input_pgm_stream >> W >> H;

  unsigned short** image = new unsigned short*[ H ];
  for( unsigned int h = 0; h < H; ++h )
    image[ h ] = new unsigned short[ W ];
  for( unsigned int h = 0; h < H; ++h )
    for( unsigned int w = 0; w < W; ++w )
      input_pgm_stream >> image[ h ][ w ];
  delete buffer;

  // Print quadtree
  std::cout << W << " " << H << std::endl;
  QuadTree( image, 0, 0, W, H, std::cout );
  delete [] image;

  return( 0 );
}

// eof - encode_pgm.cxx
