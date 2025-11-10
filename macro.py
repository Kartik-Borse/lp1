#------------------- Pass-I -------------------
def pass1(input_lines):
    mnt = []  
    mdt = []  
    intermediate_code = []

    inside_macro = False
    mdt_index = 0
    current_macro = {}

    for line in input_lines:
        line = line.strip()

        if line == "MACRO":
            inside_macro = True
            continue

        if inside_macro:
            if line == "MEND":
                mdt.append("MEND")
                inside_macro = False
                current_macro['mdt_index'] = mdt_index
                mnt.append(current_macro)
                mdt_index = len(mdt)
                current_macro = {}
                continue

            if 'macro_name' not in current_macro:
                parts = line.split()
                current_macro = {
                    'macro_name': parts[0],
                    'params': parts[1:] if len(parts) > 1 else [],
                }
            mdt.append(line)
        else:
            intermediate_code.append(line)

    return mnt, mdt, intermediate_code

# ------------------- Pass-II -------------------
def pass2(mnt, mdt, intermediate_code):
    expanded_code = []

    for line in intermediate_code:
        parts = line.strip().split()
        if not parts:
            continue

        macro_call = parts[0]
        args = parts[1:] if len(parts) > 1 else []

        # Check if it's a macro call
        macro_found = False
        for entry in mnt:
            if entry['macro_name'] == macro_call:
                macro_found = True
                formal_params = entry['params']
                ala = dict(zip(formal_params, args))

                i = entry['mdt_index'] + 1  # Skip header
                while mdt[i] != "MEND":
                    expanded_line = mdt[i]
                    for formal, actual in ala.items():
                        expanded_line = expanded_line.replace(formal, actual)
                    expanded_code.append(expanded_line)
                    i += 1
                break

        if not macro_found:
            expanded_code.append(line)

    return expanded_code

# ------------------- Main Execution -------------------
if __name__ == "__main__":
    input_lines = [
        "MACRO",
        "INCR &ARG1",
        "MOV A, &ARG1",
        "ADD A, =1",
        "MOV &ARG1, A",
        "MEND",
        "START",
        "INCR X",
        "INCR Y",
        "END"
    ]

    print("\n===== Pass I =====")
    mnt, mdt, intermediate = pass1(input_lines)

    print("\nMacro Name Table (MNT):")
    for entry in mnt:
        print(entry)

    print("\nMacro Definition Table (MDT):")
    for i, line in enumerate(mdt):
        print(f"{i}: {line}")

    print("\nIntermediate Code:")
    for line in intermediate:
        print(line)

    print("\n===== Pass II =====")
    expanded = pass2(mnt, mdt, intermediate)

    print("\nFinal Expanded Code:")
    for line in expanded:
        print(line)
