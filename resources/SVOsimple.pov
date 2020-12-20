camera {
  location  <0, 0, 7>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}


triangle {
  <-3,-2, 0 >,
  <0, 2, -4 >,
  <2,-1,0 >
  pigment {color rgb <0.65, 0.4, 0.4>}
  finish {ambient 0.3 diffuse 0.4}
}
