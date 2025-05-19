#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
using namespace std;

class node
{
public:
  char el7rf;
  int freq;
  node *left, *right, *next;

  node(char c, int f)
  {
    el7rf = c;
    freq = f;
    left = NULL;
    right = NULL;
    next = NULL;
  }

  node()
  {
    el7rf = '\0';
    freq = 0;
    left = NULL;
    right = NULL;
    next = NULL;
  }
};

class linkedlist
{
public:
  node *head;
  node *tail;

  linkedlist()
  {
    head = NULL;
    tail = NULL;
  }

  ~linkedlist()
  {
    deleteTree(head);
  }

  void deleteTree(node *root)
  {
    if (root == NULL)
      return;

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
  }

  void insert(node *pnn)
  {
    if (head == NULL)
    {
      head = tail = pnn;
      return;
    }

    node *pTrav = head;
    node *pBack = NULL;

    while (pTrav != NULL && pnn->freq > pTrav->freq)
    {
      pBack = pTrav;
      pTrav = pTrav->next;
    }

    pnn->next = pTrav;
    if (pBack != NULL)
    {
      pBack->next = pnn;
    }
    else
    {
      head = pnn;
    }

    if (pTrav == NULL)
    {
      tail = pnn;
    }
  }
};

void generateCodes(node *ptrav, string code_till_this_node, string codes[], char characters[], int n)
{
  if (ptrav == NULL)
    return;

  if (ptrav->left == NULL && ptrav->right == NULL)
  {
    for (int i = 0; i < n; i++)
    {
      if (characters[i] == ptrav->el7rf)
      {
        codes[i] = code_till_this_node;
        break;
      }
    }
  }

  generateCodes(ptrav->left, code_till_this_node + "0", codes, characters, n);
  generateCodes(ptrav->right, code_till_this_node + "1", codes, characters, n);
}

void attatch_sorted_nude(int n, char characters[], int frequencies[], linkedlist &x)
{
  for (int i = 0; i < n; i++)
  {
    node *pnn = new node(characters[i], frequencies[i]);
    x.insert(pnn);
  }
}

void build_tree(linkedlist &x)
{
  while (x.head != x.tail)
  {
    node *first_node = x.head;
    node *second_node = x.head->next;
    node *new_node = new node();
    new_node->left = first_node;
    new_node->right = second_node;
    new_node->freq = first_node->freq + second_node->freq;

    x.head = x.head->next->next;
    first_node->next = NULL;
    second_node->next = NULL;

    x.insert(new_node);
  }
}

void print_frequency_and_codes(int n, char characters[], int frequencies[], string codes[])
{
  cout << "\nHuffman Codes:\n";
  for (int i = 0; i < n; i++)
  {
    cout << characters[i] << ": Frequency = " << frequencies[i]
         << ", Huffman Code = " << codes[i] << endl;
  }
}

void calc_frequency(string &text, int frequency[], char characters[], int &n_char, int frequencies[], string codes[])
{
  unsigned char c;
  for (int i = 0; i < text.length(); i++)
  {
    c = text[i];
    int ascii_num = c;
    frequency[ascii_num]++;
  }

  n_char = 0;
  for (int i = 0; i < 256; i++)
  {
    if (frequency[i] > 0)
    {
      characters[n_char] = i;
      frequencies[n_char] = frequency[i];
      n_char++;
    }
  }
}

void read(string &text,string filename)
{
  char flag_read;
  cout << "If you want to generate code from text press 't'\n"
       << "If you want to generate code from file press 'f'\n";
  flag_read = _getch();

  if (flag_read == 't')
  {
    cout << "Enter the text: \n";

    getline(cin, text);
  }
  else if (flag_read == 'f')
  {
    ifstream myreadfile(filename, ios::binary);

    if (!myreadfile.is_open())
    {
      cout << "Error: Could not open input file\n";
      return;
    }

    myreadfile.seekg(0, ios::end);
    int size = myreadfile.tellg();
    myreadfile.seekg(0, ios::beg);

    text.resize(size);
    myreadfile.read(&text[0], size);
    myreadfile.close();
  }
  else
  {
    cout << "Error: Invalid choice\n";
    read(text,filename);
  }

  if (text.empty())
  {
    cout << "Error: Empty input\n";
    read(text,filename);
  }
}

void compresscodes(int size_arr, char eltashfer[], string &allcode)
{
  char temp = 0;
  int posbit = 0;
  int j = 0;

  for (int i = 0; i < allcode.length(); ++i)
  {
    char bit = allcode[i];
    int bitValue = bit - '0';
    temp = temp | (bitValue << (7 - posbit));
    posbit++;

    if (posbit == 8)
    {
      eltashfer[j++] = temp;
      temp = 0;
      posbit = 0;
    }
  }

  if (posbit > 0)
  {
    eltashfer[j] = temp;
  }
}

void allcode_in_one_string(string codes[], string &allcode, int n)
{
  for (int i = 0; i < n; i++)
  {
    allcode += codes[i];
  }
}

void write_into_compress_file(char *eltashfer, int size_arr)
{
  ofstream mycompress_file("compress_file.txt", ios::binary);
  if (!mycompress_file.is_open())
  {
    cout << "Error: Could not open compression output file\n";
    return;
  }

  mycompress_file.write(eltashfer, size_arr);
  mycompress_file.close();
}

void decoding_compress_file(string &code_from_compress)
{
  ifstream myreadfile("compress_file.txt", ios::binary);
  if (!myreadfile.is_open())
  {
    cout << "Error: Could not open compressed file\n";
    return;
  }

  char byte;
  while (myreadfile.read(&byte, 1))
  {
    for (int bitpos = 7; bitpos >= 0; bitpos--)
    {
      if ((byte & (1 << bitpos)))
      {
        code_from_compress += 1;
      }
      else
      {
        code_from_compress += '0';
      }
    }
  }
  myreadfile.close();
}

void convert_code_to_text(string codes[], int n, char characters[],string filename)
{
  string text;
  string temp;
  string line;
string outname="outputfile ";
outname+=filename;
  ifstream myreadfile("all_input_code.txt", ios::binary);
  if (!myreadfile.is_open())
  {
    cout << "Error: Could not open input code file\n";
    return;
  }

  while (getline(myreadfile, line))
  {

    for (int i = 0; i < line.length(); i++)
    {
      char c = line[i];
      temp += c;
      for (int j = 0; j < n; j++)
      {
        if (temp == codes[j])
        {
          text += characters[j];
          temp.clear();
          break;
        }
      }
    }
  }
  myreadfile.close();

  cout << "Decoded text length: " << text.length() << " bytes\n";

  ofstream myoutfile(outname, ios::binary);
  if (!myoutfile.is_open())
  {
    cout << "Error: Could not open output file\n";
    return;
  }
int size = text.size();

  myoutfile.write(text.c_str(), size);
  myoutfile.close();

  cout << "Text successfully decoded and written to output file\n";
}

void all_input_in_one_string(string &all_input,string filename)
{
  ifstream inputfile(filename, ios::binary);
  if (!inputfile.is_open())
  {
    cout << "Error: Could not open input file\n";
    return;
  }

  inputfile.seekg(0, ios::end);
  int size = inputfile.tellg();
  inputfile.seekg(0, ios::beg);

  all_input.resize(size);
  inputfile.read(&all_input[0], size);
  inputfile.close();
}

void all_input_code(string &all_input, string codes[], int n, char characters[])
{
  ofstream mywritefile("all_input_code.txt", ios::binary);
  if (!mywritefile.is_open())
  {
    cout << "Error: Could not open output code file\n";
    return;
  }
  for (int i = 0; i < all_input.size(); i++)
  {
    char c = all_input[i];
    for (int j = 0; j < n; j++)
    {
      if (c == characters[j])
      {
        mywritefile << codes[j];
        break;
      }
    }
  }
  mywritefile.close();
}

int main()
{
  linkedlist x;
  string text;
  int frequency[256] = {0};
  char characters[256];
  int frequencies[256];
  string codes[256];
  int number_of_characters = 0;
  string allcode;
  string code_from_compress, all_input;
string filename;
cout<<"enter file name\n";
cin>>filename;
  read(text,filename);
  if (text.empty())
  {
    cout << "Error: No input provided\n";
    return 0;
  }

  calc_frequency(text, frequency, characters, number_of_characters, frequencies, codes);
  
  attatch_sorted_nude(number_of_characters, characters, frequencies, x);
  build_tree(x);
  generateCodes(x.head, "", codes, characters, number_of_characters);
  print_frequency_and_codes(number_of_characters, characters, frequencies, codes);

  allcode_in_one_string(codes, allcode, number_of_characters);
  int size_arr = (allcode.length() + 7) / 8;
  char *eltashfer = new char[size_arr];

  all_input_in_one_string(all_input,filename);
  all_input_code(all_input, codes, number_of_characters, characters);

  compresscodes(size_arr, eltashfer, allcode);
  write_into_compress_file(eltashfer, size_arr);

  decoding_compress_file(code_from_compress);
  convert_code_to_text(codes, number_of_characters, characters,filename);
}