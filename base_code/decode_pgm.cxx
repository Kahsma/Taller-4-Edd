#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// -------------------------------------------------------------------------
void DecodeQuadTree(
    unsigned short **image,
    std::istream &in,
    unsigned int w0, unsigned int h0,
    unsigned int w1, unsigned int h1)
{
  char val;
  in >> val;
  if (val != '2')
  {
    for (unsigned int h = h0; h < h1; ++h)
      for (unsigned int w = w0; w < w1; ++w)
        image[h][w] = (unsigned short)(val - '0');
  }
  else
  {
    unsigned int hw = (w1 + w0) >> 1;
    unsigned int hh = (h1 + h0) >> 1;

    DecodeQuadTree(image, in, w0, h0, hw, hh);
    DecodeQuadTree(image, in, hw, h0, w1, hh);
    DecodeQuadTree(image, in, hw, hh, w1, h1);
    DecodeQuadTree(image, in, w0, hh, hw, h1);

  } // fi
}

// -------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    std::cerr
        << "Usage: "
        << argv[0] << " <input .qt file> <name of the .pgm that wil be created>"
        << std::endl;
    return (1);

  } // fi

  // Read PGM image as a buffer
  std::ifstream input_qt_file(
      argv[1], std::ios::in | std::ios::binary | std::ios::ate);
  if (!(input_qt_file.is_open()))
  {
    std::cerr
        << "Error opening file: \"" << argv[1] << "\""
        << std::endl;
    return (1);

  } // fi
  std::streampos size = input_qt_file.tellg();
  char *buffer = new char[size];
  input_qt_file.seekg(0, std::ios::beg);
  input_qt_file.read(buffer, size);
  input_qt_file.close();

  // Read PGM data
  std::stringstream input_qt_stream(buffer);

  // Width and height
  unsigned short W, H;
  input_qt_stream >> W >> H;

  // Image
  unsigned short **image = new unsigned short *[H];
  for (unsigned int h = 0; h < H; ++h)
    image[h] = new unsigned short[W];

  DecodeQuadTree(image, input_qt_stream, 0, 0, W, H);

  delete buffer;

  std::ofstream output_pgm_file(argv[2]);
  if (!(output_pgm_file.is_open()))
  {
    std::cerr << "Error opening output file: \"salida.pgm\"" << std::endl;
    return (1);
  }
  output_pgm_file << "P1" << std::endl;
  output_pgm_file << "# Creator: quadtree-to-image" << std::endl;
  output_pgm_file << W << " " << H << std::endl;
  for (unsigned int h = 0; h < H; ++h)
  {
    for (unsigned int w = 0; w < W; ++w)
      output_pgm_file << image[h][w] << " ";
    output_pgm_file << std::endl;

  } // rof
  output_pgm_file.close();
  delete[] image;

  return (0);
}

// eof - decode_pgm.cxx
