import re
from collections import defaultdict
import numpy as np

def parse_line(line):
    time_match = re.search(r'\(\+(0.(\d+))\)', line)
    offset_match = re.search(r'ino = (\d+), state = (\d+), flags = (\d+)', line)
    if offset_match and time_match:
        ino_number = int(offset_match.group(1))
        state = int(offset_match.group(2))
        flag = int(offset_match.group(3))
        time = float(time_match.group(1))
        return  ino_number, state, flag, time
    return None, None, None, None

def summarize_data(input_file, output_file):
    data_line = []
    time_elapseds = []
    
    with open(input_file, 'r') as f:
        for line in f:
            ino_number, state, flag, time = parse_line(line)
            if ino_number is not None and state is not None and flag is not None :
                data_line.append([ino_number,state,flag,time])
                time_elapseds.append(time)
                
                
    culmulative = np.cumsum(time_elapseds)
    with open(output_file, 'w') as f:
        f.write("Ino,State,flag,transaction,time elapsed,culmulative time elapsed\n")
        i=0
        for ino_number,state,flag,time in data_line:
            f.write(f"{ino_number},{state},{flag},{1/time},{time},{culmulative[i]}\n")
            i+=1


if __name__ == "__main__":
    input_file = 'p.txt'
    output_file = 'y.txt'
    summarize_data(input_file, output_file)