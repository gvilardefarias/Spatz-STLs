import numpy as np

class OpCoverage:
    threshold = 1000

    coverage = {"f8" :{"mul":{"a":{0:[0]*8, 1:[0]*8},
                              "b":{0:[0]*8, 1:[0]*8},
                              "o":{0:[0]*8, 1:[0]*8}},
                       "add":{"a":{0:[0]*8, 1:[0]*8},
                              "b":{0:[0]*8, 1:[0]*8},
                              "o":{0:[0]*8, 1:[0]*8}},
                       "sub":{"a":{0:[0]*8, 1:[0]*8},
                              "b":{0:[0]*8, 1:[0]*8},
                              "o":{0:[0]*8, 1:[0]*8}}},
                "f16":{"mul":{"a":{0:[0]*16, 1:[0]*16},
                              "b":{0:[0]*16, 1:[0]*16},
                              "o":{0:[0]*16, 1:[0]*16}},
                       "add":{"a":{0:[0]*16, 1:[0]*16},
                              "b":{0:[0]*16, 1:[0]*16},
                              "o":{0:[0]*16, 1:[0]*16}},
                       "sub":{"a":{0:[0]*16, 1:[0]*16},
                              "b":{0:[0]*16, 1:[0]*16},
                              "o":{0:[0]*16, 1:[0]*16}}},
                "f32":{"mul":{"a":{0:[0]*32, 1:[0]*32},
                              "b":{0:[0]*32, 1:[0]*32},
                              "o":{0:[0]*32, 1:[0]*32}},
                       "add":{"a":{0:[0]*32, 1:[0]*32},
                              "b":{0:[0]*32, 1:[0]*32},
                              "o":{0:[0]*32, 1:[0]*32}},
                       "sub":{"a":{0:[0]*32, 1:[0]*32},
                              "b":{0:[0]*32, 1:[0]*32},
                              "o":{0:[0]*32, 1:[0]*32}}},
                "f64":{"mul":{"a":{0:[0]*64, 1:[0]*64},
                              "b":{0:[0]*64, 1:[0]*64},
                              "o":{0:[0]*64, 1:[0]*64}},
                       "add":{"a":{0:[0]*64, 1:[0]*64},
                              "b":{0:[0]*64, 1:[0]*64},
                              "o":{0:[0]*64, 1:[0]*64}},
                       "sub":{"a":{0:[0]*64, 1:[0]*64},
                              "b":{0:[0]*64, 1:[0]*64},
                              "o":{0:[0]*64, 1:[0]*64}}}}

    def compute_var_coverage(self, format, operation, a_arr, b_arr, o_arr):
        if format == "f8":
            SEW = 8
        elif format == "f16":
            SEW = 16
        elif format == "f32":
            SEW = 32
        elif format == "f64":
            SEW = 64

        for i in range(len(a_arr)):
            a = bin(a_arr[i]).replace('0b', '')
            b = bin(b_arr[i]).replace('0b', '')
            o = bin(o_arr[i]).replace('0b', '')

            a = a.zfill(SEW)
            b = b.zfill(SEW)
            o = o.zfill(SEW)

            for bit in range(SEW):
                if a[bit] == '0':
                    self.coverage[format][operation]["a"][0][bit] = min(self.coverage[format][operation]["a"][0][bit] + 1, self.threshold)
                else:
                    self.coverage[format][operation]["a"][1][bit] = min(self.coverage[format][operation]["a"][1][bit] + 1, self.threshold)

                if b[bit] == '0':
                    self.coverage[format][operation]["b"][0][bit] = min(self.coverage[format][operation]["b"][0][bit] + 1, self.threshold)
                else:
                    self.coverage[format][operation]["b"][1][bit] = min(self.coverage[format][operation]["b"][1][bit] + 1, self.threshold)

                if o[bit] == '0':
                    self.coverage[format][operation]["o"][0][bit] = min(self.coverage[format][operation]["o"][0][bit] + 1, self.threshold)
                else:
                    self.coverage[format][operation]["o"][1][bit] = min(self.coverage[format][operation]["o"][1][bit] + 1, self.threshold)

        return self.coverage

    def get_coverage(self):
        sum = 0

        for format in self.coverage.keys():
            for operation in self.coverage[format].keys():
                for var in self.coverage[format][operation].keys():
                    for bit in range(len(self.coverage[format][operation][var][0])):
                        sum += self.coverage[format][operation][var][0][bit]
                        sum += self.coverage[format][operation][var][1][bit]

        return sum

    def compute_float_coverage(self, a, b):
        a_64 = a.view(np.uint64)
        b_64 = b.view(np.uint64)
        a_32 = []
        b_32 = []
        a_16 = []
        b_16 = []
        a_8  = []
        b_8  = []

        for i in range(len(a_64)):
            a_bin = bin(a_64[i]).replace('0b', '')
            b_bin = bin(b_64[i]).replace('0b', '')
            a_bin = a_bin.zfill(64)
            b_bin = b_bin.zfill(64)

            for j in range(8):
                if j < 2:
                    a_32.append(int(a_bin[j*32:(j+1)*32], 2))
                    b_32.append(int(b_bin[j*32:(j+1)*32], 2))
                if j < 4:
                    a_16.append(int(a_bin[j*16:(j+1)*16], 2))
                    b_16.append(int(b_bin[j*16:(j+1)*16], 2))

                a_8.append(int(a_bin[j*8:(j+1)*8], 2))
                b_8.append(int(b_bin[j*8:(j+1)*8], 2))

        a_f64 = np.array(a_64, dtype=np.uint64).view(np.float64)
        b_f64 = np.array(b_64, dtype=np.uint64).view(np.float64)
        a_f32 = np.array(a_32, dtype=np.uint32).view(np.float32)
        b_f32 = np.array(b_32, dtype=np.uint32).view(np.float32)
        a_f16 = np.array(a_16, dtype=np.uint16).view(np.float16)
        b_f16 = np.array(b_16, dtype=np.uint16).view(np.float16)
        a_f8  = np.array(a_8, dtype=np.uint8).view(np.float16)  # TODO: Change to float8 when supported
        b_f8  = np.array(b_8, dtype=np.uint8).view(np.float16)

        # O computation - perform operations while suppressing overflow/invalid warnings
        for operation in self.coverage["f64"].keys():
            with np.errstate(over='ignore', invalid='ignore'):
                if operation == "mul":
                    o_f64 = a_f64 * b_f64
                    o_f32 = a_f32 * b_f32
                    o_f16 = a_f16 * b_f16
                    o_f8  = a_f8  * b_f8
                elif operation == "add":
                    o_f64 = a_f64 + b_f64
                    o_f32 = a_f32 + b_f32
                    o_f16 = a_f16 + b_f16
                    o_f8  = a_f8  + b_f8
                elif operation == "sub":
                    o_f64 = a_f64 - b_f64
                    o_f32 = a_f32 - b_f32
                    o_f16 = a_f16 - b_f16
                    o_f8  = a_f8  - b_f8

            o_64 = np.array(o_f64, dtype=np.float64).view(np.uint64)
            o_32 = np.array(o_f32, dtype=np.float32).view(np.uint32)
            o_16 = np.array(o_f16, dtype=np.float16).view(np.uint16)
            o_8  = np.array(o_f8, dtype=np.float16).view(np.uint8)  # TODO: Change to float8 when supported

            self.compute_var_coverage("f64", operation, a_64, b_64, o_64)
            self.compute_var_coverage("f32", operation, a_32, b_32, o_32)
            self.compute_var_coverage("f16", operation, a_16, b_16, o_16)
            self.compute_var_coverage("f8", operation, a_8, b_8, o_8)

        return self.get_coverage()