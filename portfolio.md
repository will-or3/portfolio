# William Arnold

**Freshman at Rio Rancho High School | Aspiring Cybersecurity Researcher & Systems Programmer**

I enjoy exploring low-level systems, reverse engineering, and capture-the-flag challenges. My goal is to push system-level operations conceptually and understand software and hardware mechanics.

---

## Tech Skills

**Programming:** C, Python, ASM (x86/64 basics for CTF's)  
**Cybersecurity:** Binary Exploitation (CTFs, gdb/gef), Malware Analysis (sandboxed experiments), Reverse Engineering (Ghidra, small binaries) 
**Tools & Environments:** Linux, Windows, Ghidra, GDB?GEF


## Depth Project: Windows USB System Automation & Drive Interaction
[-> GitHub Repo <-](https://github.com/will-or3/portfolios/blob/main/WinApi_IO_Event_Logic.c)

**Note:** Developed and tested within a secure, isolated VM environment to explore advanced kernel-adjacent system mechanics.

**Goal:** To explore low-level Windows API, Implemeting a C tool that moniters and interacts with removable & non-removable storage devices via "DeviceIoControl", direct kernel-adjacent physical disk access

**What I learned / explored:**
- Windows API usage, privilege checking
- Kernel-adjacent disk interaction
- System automation via `schtask` & `winevt.h`
- Disk monitoring (`GetLogicalDrives`, `GetDiskFreeSpaceEx`)
- Disk interaction using `DeviceIoControl`, `ntdddisk.h`, `winioctl.h`

**Highlighted Snippets:**

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
## Breadth Project: Defensive Mitigations for Vulnerable Software
## ! This is not Finished !
**Overview:**  
Even well-tested software can contain vulnerabilities. This project explores defensive strategies that protect software **without modifying the original vulnerable code**

**Key Learning Areas:**

- **Understanding how attackers exploit vulnerabilities**, such as:  
  - Stack-based buffer overflows  
  - Use-after-free errors  
  - Format string vulnerabilities

- **Exploring defensive mitigation techniques**:  
  - Pointer obfuscation  
  - Runtime integrity checks  
  - Anti-debug / anti-tamper methods
  - Pre-runtime obfuscation / encryption  
  - Checksum and hash verification for critical data


### Contact
Email: will.or3@proton.me
GitHub: github.com/will-or3
