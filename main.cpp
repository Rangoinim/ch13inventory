// Michael Miller
// read/write binary record

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

struct Info
{
    char description[21];
    int qty;
    double wholesale;
    double retail;
    char date[19];
};

void print_person(Info p)
{
    std::cout << "name  : " << p.name << std::endl;
    std::cout << "age   : " << p.age << std::endl;
    std::cout << "gender: " << p.gender << std::endl;
}

void get_inventory_count(std::istream &in)
{
    long pos = in.tellg();
    in.seekg(0, std::ios::end);
    in.clear();
    std::cout << "# records = "
              << in.tellg()/sizeof(Info)
              << std::endl;
    in.seekg(pos, std::ios::beg);
}

void write_records();
void read_all_records();
void read_write_record();


const char filename[] = "inventory.dat";

int main()
{
    write_records();

    read_all_records();

    read_write_record();

    read_all_records();

    return 0;
}

void write_records()
{
    std::string desc, date;
    Info userInv;
    std::cout << "Please enter the description of the item to store in inventory (20 characters or less, including spaces): ";
    std::getline(std::cin, desc);
    while (desc.size() > 20)
    {
        std::cout << "The description is over 20 characters. Please try again: ";
        std::getline(std::cin, desc);
    }
    std::strcpy(userInv.description, desc.c_str());
    std::cout << "How many of the item are in inventory?: ";
    std::cin >> userInv.qty;
    while (userInv.qty < 0)
    {
        std::cout << "There must be at least one item in inventory in order to store it. Please try again: ";
        std::cin >> userInv.qty;
    }
    std::cout << "What is the wholesale cost of the item?: ";
    std::cin >> userInv.wholesale;
    while (userInv.wholesale < 0)
    {
        std::cout << "Please input a price above $0: ";
        std::cin >> userInv.wholesale;
    }
    std::cout << "What is the retail cost of the item? :";
    std::cin >> userInv.retail;
    while (userInv.retail < 0)
    {
        std::cout << "Please input a price above $0: ";
        std::cin >> userInv.retail;
    }
    std::cout << "Please enter the full date the item was inserted. Format: FullMonth Day, Year Eg: July 9, 2004"

    // write a file
    std::ofstream outfile(filename, std::ios::binary);

    outfile.write(reinterpret_cast<char *>(&userInv),
                  sizeof(Info));
    /*outfile.write(reinterpret_cast<char *>(&p1),
                  sizeof(Info));
    outfile.write(reinterpret_cast<char *>(&p2),
                  sizeof(Info));
    p2.age++;
    outfile.write(reinterpret_cast<char *>(&p2),
                  sizeof(Info));*/

    outfile.close();
    std::cout << "Wrote inventory to file" << std::endl << std::endl;
}

void read_all_records()
{
    // read a file
    Info p;
    std::cout << "Reading from file " << filename << std::endl;
    std::ifstream infile(filename, std::ios::binary);
    std::cout << "pos = " << infile.tellg() << std::endl;

    infile.read(reinterpret_cast<char *>(&p),
                sizeof(Info));
    while (!infile.fail())
    {
        print_person(p);
        std::cout << "pos = " << infile.tellg() << std::endl;
        infile.read(reinterpret_cast<char *>(&p),
                    sizeof(Info));
    }
    std::cout << std::endl;
    std::cout << "pos = " << infile.tellg() << std::endl;

    // did go to EOF
    infile.clear();
    std::cout << "size (bytes)/last location = " << infile.tellg() << std::endl;
    infile.close();
}

// g is for get (read) and p is for put (write)
// some implementations maintain two pointers,
// some implementations only have one pointer shared by both
void read_write_record()
{
    std::cout << std::endl << "opening r/w file " << filename << std::endl;
    Info p;
    std::fstream rwfile(filename, std::ios::in|std::ios::out|std::ios::binary);
    get_inventory_count(rwfile);

    // goes to fourth record
    std::cout << "read fourth record" << std::endl;
    rwfile.seekg(sizeof(Info)*3, std::ios::beg);
    std::cout << "pos read = " << rwfile.tellg() << std::endl;
    std::cout << "pos write = " << rwfile.tellp() << std::endl << std::endl;

    rwfile.read(reinterpret_cast<char *>(&p),
                sizeof(Info));
    print_person(p);
    std::cout << "pos read = " << rwfile.tellg() << std::endl;
    std::cout << "pos write = " << rwfile.tellp() << std::endl << std::endl;

    // at EOF so need to clear EOF again
    rwfile.clear();

    std::cout << "overwrite first record with last record contents" << std::endl;
    rwfile.seekp(0, std::ios::beg);
    rwfile.write(reinterpret_cast<char *>(&p),
                 sizeof(Info));
    std::cout << "pos read = " << rwfile.tellg() << std::endl;
    std::cout << "pos write = " << rwfile.tellp() << std::endl << std::endl;

    std::cout << "read first record" << std::endl;
    rwfile.seekg(0, std::ios::beg);
    rwfile.read(reinterpret_cast<char *>(&p),
                sizeof(Info));
    print_person(p);
    std::cout << "pos read = " << rwfile.tellg() << std::endl;
    std::cout << "pos write = " << rwfile.tellp() << std::endl << std::endl;

    // still only four records

    get_inventory_count(rwfile);
    rwfile.close();
}