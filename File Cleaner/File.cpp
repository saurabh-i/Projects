#include<bits/stdc++.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/stat.h>

using namespace std;

bool fileCheck(string file1 , string file2)
{

    int fd1 = open(file1.c_str() , O_RDWR);
    int fd2 = open(file2.c_str() , O_RDWR);

    char Buffer1[256] ,Buffer2[256] ;


        while(true)
        {
            int temp = read(fd1 , Buffer1 , 256 );
            int temp2 = read(fd2 , Buffer2 , 256);

            if(temp == 0 && temp2 == 0) break;

            int temp3 =  memcmp(Buffer1 , Buffer2 , temp);
            
            if(temp3 != 0)
            {
                return false;
            }
        }

    return true;
}

int main(int args , char * argv[])
{
    char name[256] , fileName[256];

    DIR * dp = NULL;
    struct dirent * entry = NULL;
    struct stat sobj;
    vector<string> fileNames;
    vector<int> sizes;

    cout<<"Enter the Folder name";
    cin>>name;
 
    dp = opendir(name );

    if(dp == NULL) 
    {
        cout<<"\n Unable to open folder";
        return -1;
    }

    while(true)
    {
        entry = readdir(dp);
        if(entry == NULL)
        {
            break;
        }
        sprintf(fileName ,"%s/%s", name , entry->d_name);
        stat(fileName , &sobj);

        if(S_ISREG(sobj.st_mode))
        {
            fileNames.emplace_back(fileName);
            sizes.emplace_back(sobj.st_size);
        }

    }

    for(int i = 0 ; i < sizes.size() ; i++)
    {
        cout<<"\n\t File name :"<<fileNames[i]<<"\t\tFile size :"<<sizes[i]<<"\n";
    }

    //If file size is zero then delete
    cout<<"\n---------------------------------------File size 0 operaction---------------------------------------\n";
    for(int i = 0 ; i < sizes.size() ; i++)
    {
        if(sizes[i] == 0)
        {
            cout<<"\n File Detected :";
            cout<<"\n\t File name :"<<fileNames[i]<<"\tFile size :"<<sizes[i]<<"\n";
            unlink(fileNames[i].c_str());
        }
    }
    cout<<"\n----------------------------------File size 0 operaction Completed ----------------------------------\n";
    //Display the elements having same size
    //vectors - fileNames , sizes

    // unordered_map < int , string > Hash;

    // for(int i = 0 ; i < sizes.size() ; i++)
    // {
    //     if(Hash[i].first == 0)
    //     {
    //         Hash.emplace( fileNames , sizes);
    //     }
    // }
    int Hash[1000] = {0};
    bool bret = false;
    int op = 1;

    for(int i = 0 ; i < sizes.size() ; i++)
    {
        if(Hash[sizes[i]] == 0)
        {
            Hash[sizes[i]] = i;
        }else{
            cout<<"\n  Opeaction no :"<<op;
            op++;
            cout<<"\n\t|--> File Name : |"<<fileNames[i]<<"| File size : "<<sizes[i]<<" And "<<"File Name :|"<<fileNames[Hash[sizes[i]]]<<"| File size : "<<sizes[Hash[sizes[i]]]<<" To check <-|\n";

            bret = fileCheck(fileNames[i] , fileNames[Hash[sizes[i]]] );
            //if true meanse both are same files having data same Delete one file
           
            if(bret == true)
            {
                
                cout<<"\n \t -->File to be deleted having same Data: "<<fileNames[i]<<"\n";
                unlink(fileNames[i].c_str());
                
            }else{
                //cout<<"\nFirst File : "<<fileNames[i] <<"Second File :"<< fileNames[Hash[sizes[i]]]<<" Both files are different Data. \n" ;
                cout<<"\n\t -->Both files are different ";
            }
            
            //if false meanse both are different data keep both files 
            
            
        }
    }



    cout<<"End";

    return 0;
}


/*
    Show all files Regular files
    all file names are stored in vector

    */
