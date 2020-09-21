camera {
location  <0, 0, 7>
up        <0,  1,  0>
right     <1.33333, 0,  0>
look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

plane {<0, 1, 0>, -4
      pigment {color rgb <0.2, 0.6, 0.8>}
      finish {ambient 0.4 diffuse 0.8}
}

// First Layer
sphere { <-10, -2, -10>, 2
  pigment { color rgb <1.0, 0.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.1 reflection 0.1}
}

sphere { <-5, -2, -10>, 2
  pigment { color rgb <0.3, 0.7, 0.3>}
  finish {ambient 0.2 diffuse 0.4 specular 0.6 reflection 0.3}
}

sphere { <0, -2, -10>, 2
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4 refraction 1.0 ior 1.33}
}

sphere { <5, -2, -10>, 2
  pigment { color rgb <0.2, 0.2, 1.0>}
  finish {ambient 0.2 diffuse 0.4 reflection 0.3}
}

sphere { <10, -2, -10>, 2
  pigment { color rgb <0.2, 0.6, 1.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.1 reflection 0.1}
}

// Second Layer

sphere { <-7.5, 3, -10>, 2
  pigment { color rgb <1.0, 1.0, 0.0>}
  finish {ambient 0.2 diffuse 0.7 specular 0.1 reflection 0.3 refraction 1.0 ior 1.5}
}

sphere { <-2.5, 3, -10>, 2
  pigment { color rgb <0.8, 0.2, 0.5>}
  finish {ambient 0.2 diffuse 0.4 specular 0.5 refraction 0.8 ior 1.2 reflection 0.3}
}

sphere { <2.5, 3, -10>, 2
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.1 reflection 0.8}
}

sphere { <7.5, 3, -10>, 2
  pigment { color rgb <0.5, 0, 0.5>}
  finish {ambient 0.2 diffuse 0.4 specular 0.1refraction 1.0 ior 1.3333}
}

// Third Layer

sphere { <-5, 8, -10>, 2
  pigment { color rgb <0.2, 0.5, 0.1>}
  finish {ambient 0.2 diffuse 0.7 specular 0.1}
}

sphere { <0, 8, -10>, 2
  pigment { color rgb <0.3, 0.7, 0.7>}
  finish {ambient 0.2 diffuse 0.4 specular 0.5 refraction 0.8 ior 1.2 reflection 0.3}
}

sphere { <5, 8, -10>, 2
  pigment { color rgb <1.0, 0.5, 0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.1 reflection 0.2}
}

sphere { <0, 5, -30>, 10
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.4 specular 0.1 reflection 0.8}
}

