GLOBAL _NRTC_get_hours
GLOBAL _NRTC_get_mins
GLOBAL _NRTC_get_seconds

section .text

_NRTC_get_hours:

    mov rax, 0x04
    call _RTC

    ret

_NRTC_get_mins:

    mov rax, 0x02
    call _RTC
    
    ret

_NRTC_get_seconds:
    
    mov rax, 0x00
    call _RTC

    ret

_RTC:
    out 70h, al
    in al, 71h
    ret
