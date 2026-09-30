#!/usr/bin/env python3
from __future__ import annotations
import argparse, struct
from pathlib import Path
import numpy as np
import soundfile as sf

MAGIC=b'NOMORAW1'

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('input',type=Path)
    ap.add_argument('output',type=Path)
    ap.add_argument('--peak',type=float,default=0.5)
    args=ap.parse_args()
    data,sr=sf.read(str(args.input),always_2d=True,dtype='float32')
    if data.shape[1]==1:
        data=np.repeat(data,2,axis=1)
    elif data.shape[1]>2:
        data=data[:,:2]
    peak=float(np.max(np.abs(data))) if data.size else 0.0
    target=max(0.01,min(float(args.peak),0.99))
    if peak>0.0:
        data=data*(target/peak)
    data=np.ascontiguousarray(data.T,dtype='<f4')
    args.output.parent.mkdir(parents=True,exist_ok=True)
    with args.output.open('wb') as f:
        f.write(struct.pack('<8sIIQ',MAGIC,int(sr),2,int(data.shape[1])))
        f.write(data.tobytes(order='C'))
    print(f'{args.input} -> {args.output} sr={sr} frames={data.shape[1]} source_peak={peak:.6f} target_peak={target:.3f}')
if __name__=='__main__': main()
