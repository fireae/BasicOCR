file = arg[1] or "images/cadastral.png"

bits.change_blit(1)

dofile("bitutil.lua")

image = bytearray:new()
read_image_gray(image,file)

ri_orig = RLEImage:new()
rle.convert(ri_orig,image)
rle.invert(ri_orig)
ri = RLEImage:new()
bi_orig = BitImage:new()
rle.convert(bi_orig,ri_orig)
bi = BitImage:new()

pix_orig = lepton.of_array(image)
pix_orig = lepton.pixConvertTo1(pix_orig,128)
pix_orig = lepton.pixInvert(pix_orig,pix_orig)

conditions = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 32, 64, 128 }
-- print("file size rle conv combo lept")
for i,r in ipairs(conditions) do
    ri:copy(ri_orig)
    t_rle = time(function() 
                    rle.erode_rect(ri,r,r)
                    rle.dilate_rect(ri,r,r)
                 end)
    t_conv = time(function() 
                rle.convert(bi,ri_orig)
                bits.erode_rect(bi,r,r) 
                bits.dilate_rect(bi,r,r) 
                rle.convert(ri,bi)
               end)
    t_conv_b = time(function() 
                rle.convert(ri,bi_orig)
                rle.erode_rect(ri,r,r)
                rle.dilate_rect(ri,r,r)
                rle.convert(bi,ri)
               end)
    pix = lepton.pixClone(pix_orig)
    t_lept = time(function()
                     lepton.pixErodeCompBrick(pix,pix,r,r)
                     lepton.pixDilateCompBrick(pix,pix,r,r)
                  end)
    lepton.pixDestroy(pix)
    ni = ri_orig:number_of_runs()
    no = ri:number_of_runs()
    print(string.format("%-30s %3d %10.6g %10.6g %10.6g %10.6g %6d %6d",file,r,t_rle,t_conv,t_conv_b,t_lept,ni,no))
    io.flush()
end
