AudioSignalResClass {
 Inputs {
  IOPItemInputClass {
   id 1
   name "weapon_suppressed"
   tl -1036 -91
   children {
    2 26 28
   }
  }
  IOPItemInputClass {
   id 22
   name "Distance"
   tl -1033 100
   children {
    26
   }
   valueMax 64
  }
 }
 Ops {
  IOPItemOpSubClass {
   id 9
   name "Sub 9"
   tl -588 103
   children {
    10
   }
   inputs {
    ConnectionClass connection {
     id 26
     port 0
    }
   }
   Subtracter 0.95
  }
  IOPItemOpAbsClass {
   id 10
   name "Abs 10"
   tl -390 103
   children {
    11
   }
   inputs {
    ConnectionClass connection {
     id 9
     port 0
    }
   }
  }
  IOPItemOpClampClass {
   id 11
   name "Clamp 11"
   tl -176 104
   children {
    6
   }
   inputs {
    ConnectionClass connection {
     id 10
     port 0
    }
   }
  }
  IOPItemOpInterpolateClass {
   id 26
   name "Interpolate 26"
   tl -816 102
   children {
    9
   }
   inputs {
    ConnectionClass connection {
     id 1
     port 4
    }
    ConnectionClass connection {
     id 22
     port 0
    }
   }
   "X max" 64
  }
  IOPItemOpSubClass {
   id 28
   name "Sub 9"
   tl -583 -14
   children {
    29
   }
   inputs {
    ConnectionClass connection {
     id 1
     port 0
    }
   }
  }
  IOPItemOpAbsClass {
   id 29
   name "Abs 10"
   tl -386 -15
   children {
    27
   }
   inputs {
    ConnectionClass connection {
     id 28
     port 0
    }
   }
  }
 }
 Outputs {
  IOPItemOutputClass {
   id 2
   name "suppressed"
   tl 81 -102
   input 1
  }
  IOPItemOutputClass {
   id 6
   name "suppressed_volsub"
   tl 79 99
   input 11
  }
  IOPItemOutputClass {
   id 27
   name "suppressed_invert"
   tl 80 -19
   input 29
  }
 }
 compiled IOPCompiledClass {
  visited {
   133 5 519 647 262 391 7 135 263 134 6
  }
  ins {
   IOPCompiledIn {
    data {
     3 2 196611 262147
    }
   }
   IOPCompiledIn {
    data {
     1 196611
    }
   }
  }
  ops {
   IOPCompiledOp {
    data {
     1 65539 2 196609 0
    }
   }
   IOPCompiledOp {
    data {
     1 131075 2 1 0
    }
   }
   IOPCompiledOp {
    data {
     1 65538 2 65537 0
    }
   }
   IOPCompiledOp {
    data {
     1 3 4 0 4 65536 0
    }
   }
   IOPCompiledOp {
    data {
     1 327683 2 0 0
    }
   }
   IOPCompiledOp {
    data {
     1 131074 2 262145 0
    }
   }
  }
  outs {
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
  }
  processed 11
  version 2
 }
}