import sys
# Converts an obj file with faces into a POV
def writeHeader(outfile):
   outfile.write("camera {\n   location <0, 0, 7>\n   up <0, 1, 0>\n   right <1.33333, 0, 0>\n")
   outfile.write("   look_at <0, 0, 0>\n}\n\n")
   outfile.write("light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}\n")

def writeSphere(center, color, outfile):
   outfile.write("sphere { <%f, %f, %f>, 0.01\n" % (center[0], center[1], center[2]))
   outfile.write("   pigment { color rgb <%f, %f, %f>}\n" % (color[0], color[1], color[2]))
   outfile.write("   finish { ambient 0.2 diffuse 0.4}\n")
   outfile.write("}\n\n")

def writeTriangle(p1, p2, p3, color, outfile):
   outfile.write("triangle {\n   <%f, %f, %f>,\n" % (p1[0], p1[1], p1[2]))
   outfile.write("   <%f, %f, %f>,\n" % (p2[0], p2[1], p2[2]))
   outfile.write("   <%f, %f, %f>\n" % (p3[0], p3[1], p3[2]))
   outfile.write("   pigment { color rgb <%f, %f, %f>}\n" % (color[0], color[1], color[2]))
   outfile.write("   finish { ambient 0.2 diffuse 0.4}\n")
   outfile.write("}\n\n")

def main():
   
   infile = open(sys.argv[1], "r")

   vertices = []
   normals = []
   textures = []
   faces = []
   vCount = vnCount = vtCount = fCount = 0
   for line in infile:
      args = line.split()
      if args[0] == 'v':
         vCount += 1
         vertices.append(args[1:])
      elif args[0] == 'vn':
         vnCount += 1
         normals.append(args[1:])
      elif args[0] == 'vt':
         vtCount += 1
         textures.append(args[1:])
      elif args[0] == 'f':
         fCount += 1
         faces.append(args[1:])
   print("V: %d\nVn: %d\nVt: %d\nFaces: %d\n" % (vCount, vnCount, vtCount, fCount))

   outfile = open(sys.argv[2], "w")
   writeHeader(outfile)

   for points in faces:
      v0 = vertices[int(points[0].split('/')[0])-1]
      v1 = vertices[int(points[1].split('/')[0])-1]
      v2 = vertices[int(points[2].split('/')[0])-1]
      r = (float(v0[3]) + float(v1[3]) + float(v2[3]))/3.0
      g = (float(v0[4]) + float(v1[4]) + float(v2[4]))/3.0
      b = (float(v0[5]) + float(v1[5]) + float(v2[5]))/3.0
      writeTriangle([float(x) for x in v0[0:3]], [float(x) for x in v1[0:3]], [float(x) for x in v2[0:3]], [r,g,b], outfile)



if __name__ == '__main__':
   main()
