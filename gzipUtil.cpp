#include <iostream>
#include <chrono>

#include "GZipFile.hpp"

using namespace std;

void usage()
{
    cout << "Usage:\n\tprogram operation source dest\n"
         << "Supported commands: \n"
         << "\tinflate file_1 file_2 - extracts the contents of file_1 and writes them to the destination specified by file_2" << endl;
}

int main(int argc, char *argv[])
{
    try
    {
        // we only support up to 3 command line arguments to be passed in
        if (argc >= 3 && argc <= 4)
        {
            if (string("inflate").compare(argv[1]) == 0)
            {
                chrono::time_point start_time{chrono::steady_clock::now()};
                // open the file, inflate it, and write it to disk
                GZipFile gzip_file(argv[2], argv[3]);
                gzip_file.inflate();
                chrono::time_point end_time{chrono::steady_clock::now()};
                chrono::duration<double> elapsed_time_seconds{end_time - start_time};
                chrono::duration<double, milli> elapsed_time_milli{end_time - start_time};
                cout << "Elapsed time: " << elapsed_time_seconds.count() << " seconds, ";
                cout << elapsed_time_milli.count() << " milliseconds, " << endl;;
            }
            else
            {
                usage();
            }
        }
        else
        {
            usage();
        }
    }
    catch (exception &e)
    {
        // log out what happend with each exception
        cerr << e.what() << endl;
    }

    return 0;
}