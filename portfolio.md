# William Arnold

**Freshman at Rio Rancho High School | building low-level tooling and exploring binary exploitation, reverse engineering, and kernel systems.**

I enjoy exploring low-level systems, reverse engineering, and capture-the-flag challenges. My goal is to push system-level operations conceptually and understand software and hardware mechanics.

---

## Tech Skills

**Programming:** C, Python, ASM (x86/64 basics for CTF's)  
**Cybersecurity:** Binary Exploitation (CTFs, gdb/gef), Malware Analysis (sandboxed experiments), Reverse Engineering (Ghidra, small binaries) 
**Tools & Environments:** Linux, Windows, Ghidra, GDB/GEF


## Windows USB System Automation & Drive Interaction
[-> GitHub Repo <-](https://github.com/will-or3/portfolios/blob/main/WinApi_IO_Event_Logic.c)

**Environment:** Windows 10 vm  
**Language:** C  
**Focus Areas:** Windows API, Disk I/O, Privilege Handling, Event-Driven Systems 

## Overveiw

This project explores how userland applications can interact with **kernel-managed disk systems** using low-level Windows APIs/

the tool monitors system drives, evaluates conditions (storage thresholds), & interactions with physical disks via 'DeviceIoControl'

### Goals:
- Understand How Windows exposes disk level operations to User-mode programs
- Apply event-driven logic to system-level workflows
- Limitations of Userland programs

## System Architecture
```text
Drive Detection -> Privilege Check -> Disk Evaluation -> Conditional Trigger -> Raw Device Interface
```

## Key Concepts Demonstrated

- Userland vs Kernel Boundaries
    Understaing how Windows mediates access to hardware through system APIs

- Raw Device Access
    Working with `\\.PhysicalDriveX` handles under OS constraints

- Event-Driven Design
    Triggering behavior based on system state (drive conditionals)

- Low-Level I/O Constraints
    Buffer alignment, unbuffered I/O, and system limitations 

## Quick featues:
- Event-driven disk monitoring
- Admin Privilege validation
- Conditional system automation
- Raw disk handle acquisition using Windows API
- controlled experimentation with low-level I/O Operations

**Technical Highlights:**

**1. Disk Interaction (Kernel-Adjacent I/O)**  
Demonstrates how Windows allows user-mode programs to interface with physical drives through controlled APIs, including handling unbuffered I/O and alignment constraints.
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
                triggered_operation();
                break;
            }
        }
    }
}
```
# Future awaits...


### Contact
Email: will.or3@proton.me

GitHub: github.com/will-or3