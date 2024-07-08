//C++ Program to create a Student management system

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;


class Student
{
protected:
    char name[30], father_name[20], mother_name[20], address[20];
    int roll_no;
    long long contact_no;
    
    void setStudentDetail()
    {
        string temp;
        cout << "\nEnter Student's Roll Number: ";
        cin >> roll_no;
        getline(cin, temp);

        cout << "Enter Student's Full Name: ";
        getline(cin, temp);
        strcpy(name, temp.c_str());
        
        cout << "Enter Student's Father's Name: ";
        getline(cin, temp);
        strcpy(father_name, temp.c_str());
        
        cout << "Enter Student's Mother's Name: ";
        getline(cin, temp);
        strcpy(mother_name, temp.c_str());

        cout << "Enter Student's Contact Number: ";
        cin >> contact_no;
        getline(cin, temp);
        
        cout << "Enter Student's Address: ";
        getline(cin, temp);
        strcpy(address, temp.c_str());
    }
};


class Test : public Student
{
protected:    
    char class_name[20], academic_year[20], test_name[20];
    
    struct Subject
    {
        char name[30];
        float mark, max_mark;
    } subject[5];

public:
    void setTestDetail()
    {
        string temp;
        setStudentDetail();

        cout << "\nEnter Class Name: ";
        getline(cin, temp);
        strcpy(class_name, temp.c_str());
        
        cout << "Enter Academic Year: ";
        getline(cin, temp);
        strcpy(academic_year, temp.c_str());
        
        cout << "Enter Test Name: ";
        getline(cin, temp);
        strcpy(test_name, temp.c_str());

        for (int i = 0; i < 5; i++)
        {
            cout << "\nEnter Subject " << i + 1 << " Name: ";
            getline(cin, temp);
            strcpy(subject[i].name, temp.c_str());
            
            cout << "Enter Subject " << i + 1 << " Mark: ";
            cin >> subject[i].mark;
            
            cout << "Enter Subject " << i + 1 << " Maximum Mark: ";
            cin >> subject[i].max_mark;
            getline(cin, temp);
        }
    }
};


class Result : public Test
{
    float mark, max_mark, percentage;
    char result[20];

public:
    void calculateResult()
    {
        mark = 0, max_mark = 0;
        bool is_fail = false;

        for (int i = 0; i < 5; i++)
        {
            if (subject[i].mark * 100.0 / subject[i].max_mark < 35)
                is_fail = true;
            
            mark += subject[i].mark;
            max_mark += subject[i].max_mark;
        }

        percentage = mark * 100.0 / max_mark;
        if (is_fail)
            strcpy (result, "Fail");
        else if (percentage >= 75)
            strcpy (result, "Pass - Distinction");
        else if (percentage >= 60)
            strcpy (result, "Pass - First Class");
        else if (percentage >= 50)
            strcpy (result, "Pass - Second Class");
        else
            strcpy (result, "Pass - Third Class");
    }

    bool operator<(Result r)    //for sorting
    {
        if (strcmp(academic_year, r.academic_year) < 0)
            return true;
        else if (strcmp(academic_year, r.academic_year) > 0)
            return false;
        else if (strcmp(test_name, r.test_name) < 0)
            return true;
        else if (strcmp(test_name, r.test_name) > 0)
            return false;
        else if (strcmp(class_name, r.class_name) < 0)
            return true;
        else if (strcmp(class_name, r.class_name) > 0)
            return false;
        else if (roll_no < r.roll_no)
            return true;
        else
            return false;
    }

    bool operator==(Result r)
    {
        return (roll_no == r.roll_no && !strcmp(academic_year, r.academic_year) && !strcmp(test_name, r.test_name));
    }

    friend class Queries;
    friend class JSON;
};


class Storage
{
protected:
    vector<Result> data;
    
    void add()
    {
        Result temp;
        temp.setTestDetail();
        temp.calculateResult();
        
        data.push_back(temp);
        cout << "\nData created successfully!!\n";
    }

    void storeToFile(string name)
    {
        ifstream fin(name, ios::binary);

        if (fin.is_open())
        {
            while (!fin.eof())
            {
                Result temp;
                fin.read((char *) &temp, sizeof(Result));
                data.push_back(temp);
            }

            fin.close();
        }
        
        sort (data.begin(), data.end());

        ofstream fout(name, ios::binary);
        
        for (int i = 0; i < data.size(); i++)
            fout.write((char *) &data[i], sizeof(Result));
        
        fout.close();
        cout << "Data is stored in file successfully\n";
    }

    void retrieveFromFile(string name)
    {
        ifstream fin(name, ios::binary);
        if (!fin.is_open())
        {
            cout << "File does not exists!!\n";
            return;
        }

        while (!fin.eof())
        {
            Result temp;
            fin.read((char *) &temp, sizeof(Result));
            data.push_back(temp);
        }

        fin.close();
        sort (data.begin(), data.end());
        data.erase(unique(data.begin(), data.end()), data.end());
        
        cout << "File is retrieved successfully\n";
    }
};


class JSON : public Storage
{
public:
    void createJSON(int roll_no, string file_name)
    {
        sort (data.begin(), data.end());
        vector <Result>::iterator iter = data.begin();

        while (true)
        {
            if (iter == data.end())
            {
                cout << "No Record exist\n";
                return;
            }

            if (iter->roll_no == roll_no)
                break;
            
            iter++;
        }

        ofstream fout(file_name);
        fout << "{\n \"Roll no\" : " << roll_no << ",\n";
        fout << " \"Full Name\" : \"" << iter->name << "\",\n";
        fout << " \"Father's Name\" : \"" << iter->father_name << "\",\n";
        fout << " \"Mother's Name\" : \"" << iter->mother_name << "\",\n";
        fout << " \"Contact Number\" : \"" << iter->contact_no << "\",\n";
        fout << " \"Address\" : \"" << iter->address << "\",\n";
        fout << " \"Academics\" : [";

        char prev_academic_year[20] = "";

        for (; iter != data.end(); iter++)
        {
            if (iter->roll_no != roll_no)
                continue;
            
            if (strcmp(prev_academic_year, iter->academic_year))
            {
                if (strcmp(prev_academic_year, ""))
                    fout << "\n\t ]\n\t},";
                
                fout << "\n\t{\n";
                fout << "\t \"Academic Year\" : \"" << iter->academic_year << "\",\n";
                fout << "\t \"Class Name\" : \"" << iter->class_name << "\",\n";
                fout << "\t \"Tests\" : [\n\t\t{\n";
            }
            
            else
                fout << ",\n\t\t{\n";

            fout << "\t\t \"Test Name\" : \"" << iter->test_name << "\",\n";
            fout << "\t\t \"Subjects\" : [";

            for (int i = 0; i < 5; i++)
            {
                fout << "\n\t\t\t{\n";
                fout << "\t\t\t \"Subject Name\" : \"" << iter->subject[i].name << "\",\n";
                fout << "\t\t\t \"Mark\" : " << iter->subject[i].mark << ",\n";
                fout << "\t\t\t \"Maximum Mark\" : " << iter->subject[i].max_mark << "\n\t\t\t}";
                
                if (i < 4)
                    fout << ",";
            }

            fout << "\n\t\t ],\n";
            fout << "\t\t \"Total Mark\" : " << iter->mark << ",\n";
            fout << "\t\t \"Total Maximum Mark\" : " << iter->max_mark << ",\n";
            fout << "\t\t \"Percentage\" : " << iter->percentage << ",\n";
            fout << "\t\t \"Result\" : \"" << iter->result << "\"\n\t\t}";
            strcpy (prev_academic_year, iter->academic_year);
        }

        fout << "\n\t ]\n\t}\n ]\n}";
        fout.close();
        cout << "JSON File succesfully created!!\n";
    }
};


class Queries : public JSON
{
    void getTopper(const char academic_year[], const char test_name[])
    {
        sort(data.begin(), data.end());
        char prev_class[20] = "";
        bool got_topper = false;
        vector<Result> topper;

        cout << "\nToppers in all class:\n";
        for (vector<Result>::iterator iter = data.begin(); iter != data.end(); iter++)
        {
            if (strcmp(iter->academic_year, academic_year) || strcmp(iter->test_name, test_name))
                if (got_topper)
                    break;
                else
                    continue;
            
            else
            {
                got_topper = true;
                if (!strcmp(prev_class, ""))
                    topper.push_back(*iter);
                
                else if (strcmp(prev_class, iter->class_name))
                {
                    for (int i = 0; i < topper.size(); i++)
                    {
                        cout << "\nTopper Class: " << topper[i].class_name << endl;
                        cout << "Topper Roll no: " << topper[i].roll_no << endl;
                        cout << "Topper Name: " << topper[i].name << endl;
                        cout << "Topper Percentage: " << topper[i].percentage << endl;
                    }

                    topper.erase(topper.begin(), topper.end());
                    topper.push_back(*iter);
                }

                else if (topper[0].percentage < iter->percentage)
                {
                    topper.erase(topper.begin(), topper.end());
                    topper.push_back(*iter);
                }

                else if (topper[0].percentage == iter->percentage)
                    topper.push_back(*iter);
                
                strcpy(prev_class, iter->class_name);
            }
        }

        if (!topper.size())
            cout << "No class exist!!\n";
        else
            for (int i = 0; i < topper.size(); i++)
            {
                cout << "\nTopper Class: " << topper[i].class_name << endl;
                cout << "Topper Roll no: " << topper[i].roll_no << endl;
                cout << "Topper Name: " << topper[i].name << endl;
                cout << "Topper Percentage: " << topper[i].percentage << endl;
            }
    }

    void getFailure(const char academic_year[], const char test_name[], const char class_name[])
    {
        cout << "\nFailures:\n";
        for (vector<Result>::iterator iter = data.begin(); iter != data.end(); iter++)
            if (!strcmp(academic_year, iter->academic_year) && !strcmp(test_name, iter->test_name) && !strcmp(class_name, iter->class_name) && !strcmp(iter->result, "Fail"))
                cout << iter->roll_no << " - " << iter->name << endl;
    }

public:
    Queries()
    {
        int choice;
        while (true)
        {
            cout << "\n1. Input Data\n2. Get Data in JSON\n3. Store Data to file\n4. Retrieve Data from file\n5. Get Topper\n6. Get Failure\n7. Exit\n";
            cout << "\nEnter your Choice: ";
            cin >> choice;
            
            string bin;
            getline(cin, bin);
            
            if (choice == 7)
                break;

            switch(choice)
            {
                case 1:
                    add();
                    break;

                case 2:
                {
                    int roll_no;
                    cout << "Enter Roll no: ";
                    cin >> roll_no;
                    
                    string file_name;
                    cout << "Enter File name: ";
                    cin >> file_name;
                    
                    createJSON(roll_no, file_name);
                    break;
                }

                case 3:
                {
                    string file_name;
                    cout << "Enter File Name: ";
                    getline(cin, file_name);

                    storeToFile(file_name);
                    break;
                }

                case 4:
                {
                    string file_name;
                    cout << "Enter File Name: ";
                    getline(cin, file_name);
                    
                    retrieveFromFile(file_name);
                    break;
                }

                case 5:
                {
                    string academic_year, test_name;
                    cout << "Enter Academic Year: ";
                    getline(cin, academic_year);
                    cout << "Enter Test Name: ";
                    getline(cin, test_name);
                    
                    getTopper(academic_year.c_str(), test_name.c_str());
                    break;
                }

                case 6:
                {
                    string academic_year, test_name, class_name;
                    cout << "Enter Academic Year: ";
                    getline(cin, academic_year);
                    cout << "Enter Test Name: ";
                    getline(cin, test_name);
                    cout << "Enter Class Name: ";
                    getline(cin, class_name);

                    getFailure(academic_year.c_str(), test_name.c_str(), class_name.c_str());
                    break;
                }

                default:
                    cout << "Invalid choice!!\n";
                    break;
            }
        }
    }
};


int main()
{
    Queries q;
    return 0;
}