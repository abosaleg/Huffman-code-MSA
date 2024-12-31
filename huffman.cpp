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

    while (pTrav != NULL && pnn->freq >= pTrav->freq)
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

  if (ptrav->left == NULL && ptrav->right == NULL) // عشان نتاكد انها ليف
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
void calc_frequency(string text, int frequency[], char characters[], int &n_char, int frequencies[], string codes[])
{
  for (int i = 0; i < text.length(); i++)
  {
    int ascii_num = text[i]; // الحرف بس ب الاسكي
    frequency[ascii_num]++;
  }
  for (int i = 0; i < 256; i++)
  {
    if (frequency[i] > 0)
    {
      char el7rf_mn_elascii = i;
      characters[n_char] = el7rf_mn_elascii;
      frequencies[n_char] = frequency[i];
      n_char++; // عدد الحروف منغير تكرار
    }
  }
}

void read(string &text)
{
  int flag_read;
  string temp = "";
  cout << "if you want to generate code from text click t  \n"
       << "if you want to generate code from file click f\n";
  flag_read = _getch();
  if ('t' == flag_read)
  {
    cout << "Enter the text: \n";
    getline(cin, text);
  }
  else if ('f' == flag_read)
  {
    ifstream myreadfile("input_file.txt");
    if (!myreadfile.is_open())
    {
      cout << "Error reading file not open \n";
      return;
    }
    while (getline(myreadfile, temp))
    {
      text += temp;
    }
  }
  else
  {
    cout << "Error Invalid choice \n";
    read(text);
  }
  if (text.empty())
  {
    cout << "Error Empty input" << endl;
    read(text);
  }
}
///////////////
void compresscodes(int size_arr, char eltashfer[], string codes, int n)
{
  char temp = 0;
  int posbit = 0;
  int j = 0;
  

    for (int k = 0; k < codes.length(); k++)
    {
      int bitValue = codes[k] - '0';
      temp = temp | (bitValue << (7 - posbit)); // Changed to store from left to right
      posbit++;
      if (posbit == 8)
      {
        eltashfer[j] = temp;
        temp = 0;
        posbit = 0;
        j++;
      }
    }    
  

  // Handle remaining bits if any
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

void write_into_compress_file(char eltashfer[], int size_arr)
{
  ofstream mycompress_filefile("compress_file.txt");
  if (!mycompress_filefile.is_open())
  {
    cout << "Error writing file not open \n";
  }
  else
  {
    for (int i = 0; i < size_arr; i++)
    {
      mycompress_filefile << eltashfer[i];
    }
  }
}

void decoding_compress_file(string &code_from_compress)
{
  ifstream myreadfile("compress_file.txt");
  if (!myreadfile.is_open())
  {
    cout << "Error reading file not open \n";
    return;
  }
  string text = "", temp = "";
  char byte;
  while (getline(myreadfile, temp))
  {
    text += temp;
  }
  for (int i = 0; i < text.length(); i++)
  {
    byte = text[i];
    for (int bitpos = 7; bitpos >= 0; bitpos--) // Read from left to right
    {
      if (byte & (1 << bitpos))
        code_from_compress += '1';
      else
        code_from_compress += '0';
    }
  }
  // cout << "The code from compress file is: " << code_from_compress << endl; // Debug print
}
void convert_code_to_text(string code_from_compress, string codes[], int n, char characters[])
{
  string text = "";
  string temp = "";

  for (int i = 0; i < code_from_compress.length(); i++)
  {
    temp += code_from_compress[i];
    for (int j = 0; j < n; j++)
    {
      if (temp == codes[j])
      {
        text += characters[j];
        temp = "";
        break;
      }
    }
  }
  cout << "The text from compress file is: " << text << endl;
  ofstream myoutfile("output_file.txt");
  if (myoutfile.is_open())
  {
    myoutfile << text;
    myoutfile.close();
  }
  else
    cout << "Unable to open file";
}
void all_input_in_one_string(string &all_input)
{
  string line = "";
  ifstream inputfile("input_file.txt");
  if (!inputfile.is_open())
  {
    cout << "Error reading file not open \n";
    return;
  }
  while (getline(inputfile, line))
  {
    all_input += line;
  }
  cout << "The input from input file is:   " << all_input << endl;
}
string  all_input_code(string all_input_in_one_string, string codes[], int n, char characters[])
{
  string all_input_code = "";
  for (int i = 0; i < all_input_in_one_string.length(); i++)
  {
    for (int j = 0; j < n; j++)
    {
      if (all_input_in_one_string[i] == characters[j])
      {
        all_input_code += codes[j];
        break;
      }
    }
  }
  // cout << "The code from input file is:  " << all_input_code << endl; // Debug print
  return all_input_code;
}

int main()
{
  linkedlist x;
  string text;
  int frequency[256] = {0};
  char characters[256];
  int frequencies[256];
  string codes[256] = {""};
  int number_of_characters = 0;
  string allcode;
  string code_from_compress = "",all_input="";
  read(text);
  calc_frequency(text, frequency, characters, number_of_characters, frequencies, codes);

  attatch_sorted_nude(number_of_characters, characters, frequencies, x);

  build_tree(x);

  generateCodes(x.head, "", codes, characters, number_of_characters);

  print_frequency_and_codes(number_of_characters, characters, frequencies, codes);
  allcode_in_one_string(codes, allcode, number_of_characters);
  int size_arr = (allcode.length() + 7) / 8;
  char eltashfer[size_arr];
  // cout << "All codes in one string:        " << allcode << endl; // Debug print
  all_input_in_one_string(all_input);
  string  all_in_code=all_input_code(all_input, codes, number_of_characters, characters);

  compresscodes(size_arr, eltashfer, all_in_code, number_of_characters);
  write_into_compress_file(eltashfer, size_arr);
  decoding_compress_file(code_from_compress);
  convert_code_to_text( all_in_code, codes,number_of_characters, characters);
}