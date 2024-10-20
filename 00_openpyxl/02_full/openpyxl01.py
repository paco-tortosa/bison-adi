from openpyxl import load_workbook

def is_float(v):
    try:
        f=float(v)
    except ValueError:
        return False
    return True

wb = load_workbook(filename = 'Apollo_Rx_Latency_Calculator_B0.xlsm')

# print (wb.sheetnames)
for s in wb.sheetnames:
    print("Sheet = ", "\"", s, "\"", sep="")

    ws = wb[s]
    for row in ws:
        # ['A2':'B5']:
        for c in row:
            if c.value is not None:
                if is_float(c.value):
                    print(c.coordinate, "=", c.value)
                elif c.value.startswith("="):
                    print(c.coordinate, "=", c.value[1:])
                else:
                    print(c.coordinate, " = \"", c.value, "\"", sep="")

                # if is_float(c.value):
                #     print("")
                # elif c.value.startswith("="):
                #     print(c.coordinate, "=", c.value[1:])

print("")
print("defined names :")
for k, v in wb.defined_names.items():
    print(k, "alias", v.attr_text.replace('$',''), "\"")
