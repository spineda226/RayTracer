import sys

def writeHeader(outfile):
   outfile.write("camera {\n   location <0, 0, 7>\n   up <0, 1, 0>\n   right <1.33333, 0, 0>\n")
   outfile.write("   look_at <0, 0, 0>\n}\n\n")
   outfile.write("light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}\n")

def writeSphere(center, color, outfile):
   outfile.write("sphere { <%f, %f, %f>, 0.01\n" % (center[0], center[1], center[2]))
   outfile.write("   pigment { color rgb <%f, %f, %f>}\n" % (color[0], color[1], color[2]))
   outfile.write("   finish { ambient 0.2 diffuse 0.4}\n")
   outfile.write("}\n\n")

def main():
   infile = open(sys.argv[1], "r")
   outfile = open(sys.argv[2], "w")
   writeHeader(outfile)
   for line in infile:
      nums = line.split()
      center = [float(x) for x in nums[0:3]]
      color = [int(x)/255.0 for x in nums[3:6]]
      writeSphere(center, color, outfile)

if __name__ == '__main__':
   main()
