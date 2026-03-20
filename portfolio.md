# William Arnold

**Freshman at Rio Rancho High School | building low-level tooling and exploring binary exploitation, reverse engineering, and kernel systems.**

I enjoy exploring low-level systems, reverse engineering, and capture-the-flag challenges. My goal is to push system-level operations conceptually and understand software and hardware mechanics.

---

## Tech Skills

**Programming:** C, Python, ASM (x86/64 basics for CTF's)  
**Cybersecurity:** Binary Exploitation (CTFs, gdb/gef), Malware Analysis (sandboxed experiments), Reverse Engineering (Ghidra, small binaries) 
**Tools & Environments:** Linux, Windows, Ghidra, GDB?GEF


## Depth Project: Windows USB System Automation & Drive Interaction
[-> GitHub Repo <-](https://github.com/will-or3/portfolios/blob/main/WinApi_IO_Event_Logic.c)

**Environment:** Windows 10 vm  
**Language:** C  
**Focus Areas:** Windows API, Disk I/O, Privilege Handling, Event-Driven Systems 

## Overveiw

This project explores how userland applications can interact with **kernel-managed-disk systems** using low-level Windows APIs/

the tool monitors system drives, evaluates conditions (storage thresholds), & interactions with physical disks via 'DeviceIoControl'

Goals:
- How Windows exposes disk level operations to User level apis
- How event driven logic can be applied to system level programming
- Limitations of Userland programs

## System Architecture
```text
Drive Detection -> Privilege Check -> Disk Evaluation -> Conditional Trigger -> Low-Level Disk Access
```
## Components
- Drive Detection
    uses `GetLogicalDrives` to enumeraate available storage devices
- Privilege check
    Verifies admin privs before performing sensitive operations.
- Disk evaluation
    measures disk usage via `GetDiskFreeSpaceEx` & compares against threshold
- Conditional trigger
    Executes logic when specific conditions are met (threshold exceeded)
- Low-Level Disk Access
    Interacts with physical drives useing `CreateFile` & `DeviceIoControl`

## Quick featues:
- Event-driven disk monitoring
- Admin Privilege validation
- Conditional system automation
- Raw disk handle acquisition using Windows API
- controlled experimentation with low-level I/O Operations

**Technical Highlights:**

**1. Disk Interaction (Kernel-Adjacent I/O)**  
Demonstrated direct disk access using Windows API to read/write raw drives conceptually while bypassing caching.
```c
HANDLE hDisk = CreateFileA(
            disk_path,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, 
            OPEN_EXISTING,
            FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH,
            0
        );
        if (hDisk == INVALID_HANDLE_VALUE) continue;
```
2. Admin Check
Verify administrator privileges and conditionally execute self-deletion.
```c
bool admin_check() {
    BOOL is_admin = FALSE;
    PSID admin_group = NULL;
    if (ConvertStringSidToSidA("S-1-5-32-544", &admin_group)) {
        if (!CheckTokenMembership(NULL, admin_group, &is_admin)) {
            is_admin = FALSE;
        }
        LocalFree(admin_group);
    }
}
if (!is_admin) {
    self_del()
}
```
3. USB Detection & Conditional Trigger
Detect USB drives and execute conceptual actions if the drive exceeds a 4GB threshold.

```c
void check_usb_threshold(ULONGLONG threshold) {
    for (int b = 0; b < 26; b++) {
            if (drive_mask & (1 << b)) {
                wchar_t drive[4] = {L'A' + b, L':', L'\\', L'\0'};
                ULONGLONG used = used_space_drive(drive);
            if (used >= threshold) {
                payload();
                break;
            }
        }
    }
}
```
# Future awaits


### Contact
Email: will.or3@proton.me

GitHub: github.com/will-or3