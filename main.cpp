#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include <Windows.h>


std::string get_random_ascii_data(bool file_name = false)
{
    const int low = file_name ? 0x61 : 0x21;
    const int high = file_name ? 0x7a : 0x7e;
    const int length = file_name ? 15 : 4096;
    std::string buffer;
    for (size_t i = 0; i < length; ++i)
        buffer.push_back((const char)(low + rand() % (high - low + 1)));

    return buffer;
}

void create_file()
{
    auto name = get_random_ascii_data(true) + ".txt";

    std::cout << "CreateFile " << name << std::endl;

    auto file_handle = CreateFileA(name.data(), FILE_APPEND_DATA, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file_handle == INVALID_HANDLE_VALUE) 
    { 
        std::cout << "Terminal failure: Unable to open file \"" << name << "\" for write" << std::endl;
        return;
    }

    for (int i = 0; i < 4000; ++i)
    {
        DWORD bytes_written = 0;
        auto buffer = get_random_ascii_data();
        auto error_flag = WriteFile(file_handle, buffer.data(), buffer.size(), &bytes_written, nullptr);

        if (FALSE == error_flag)
        {
            std::cout << "Terminal failure: Unable to write to file" << std::endl;
            break;
        }
    }

    CloseHandle(file_handle);
    DeleteFileA(name.data());
}

void create_child_processes(std::string name)
{
    for (int i = 0; i < 1000; ++i)
    {
        std::cout << "[" << i << "] CreatePocess " << name << std::endl;
        STARTUPINFOA info={sizeof(info)};
        PROCESS_INFORMATION process_info;
        auto full_cmd = name + " child";
        if (CreateProcessA(nullptr, (char*)full_cmd.data(), NULL, NULL, TRUE, 0, NULL, NULL, &info, &process_info))
        {
            std::cout << "Create process " << process_info.dwProcessId << " (" << process_info.hProcess << ")" << std::endl;
            WaitForSingleObject(process_info.hProcess, INFINITE);
            std::cout << "Process " << process_info.dwProcessId << " (" << process_info.hProcess << ") finished" << std::endl;
            CloseHandle(process_info.hProcess);
            CloseHandle(process_info.hThread);
        }
        else
        {
            std::cout << "Failed to create process" << std::endl;
        }
    }
}

void print_usage(std::string name)
{
    std::cout << "Usage: " << name << " [parent | child]" << std::endl;
}

int main(int argc, char** argv)
{
    std::srand(std::time(nullptr));
    //CreateProcessA("app.exe", "")
    std::string role;
    if (argc == 1)
        role = "parent";
    else if (argc == 2)
        role = std::string(argv[1]);
    else
    {
        print_usage(argv[0]);
        return -1;
    }
    

    std::cout << "My role is " << role << std::endl;

    if ("parent" == role)
        create_child_processes(std::string(argv[0]));
    else if ("child" == role)
        create_file();
    else
    {
        print_usage(argv[0]);
        return -1;
    }

    return 0;
}
