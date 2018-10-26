./Assignment1   --noisify 0     --in TestImages/yoda.bmp --out OutputImages/yoda_noise_0.bmp
./Assignment1   --noisify 0.1   --in TestImages/yoda.bmp --out OutputImages/yoda_noise_0.1.bmp
./Assignment1   --noisify 0.5   --in TestImages/yoda.bmp --out OutputImages/yoda_noise_0.5.bmp
 
./Assignment1   --brighten .5   --in TestImages/yoda.bmp --out OutputImages/yoda_bright_0.5.bmp
./Assignment1   --brighten 1	--in TestImages/yoda.bmp --out OutputImages/yoda_bright_1.bmp
./Assignment1   --brighten 2    --in TestImages/yoda.bmp --out OutputImages/yoda_bright_2.bmp
 
./Assignment1   --gray  --in TestImages/yoda.bmp --out OutputImages/yoda_gray.bmp
  
./Assignment1   --contrast .5   --in TestImages/shrek.bmp --out OutputImages/shrek_contrast_0.5.bmp
./Assignment1   --contrast 1    --in TestImages/shrek.bmp --out OutputImages/shrek_contrast_1.bmp
./Assignment1   --contrast 2    --in TestImages/shrek.bmp --out OutputImages/shrek_contrast_2.bmp
   
./Assignment1   --saturate .5   --in TestImages/shrek.bmp --out OutputImages/shrek_saturate_0.5.bmp
./Assignment1   --saturate 1    --in TestImages/shrek.bmp --out OutputImages/shrek_saturate_1.bmp
./Assignment1   --saturate 2    --in TestImages/shrek.bmp --out OutputImages/shrek_saturate_2.bmp

./Assignment1   --quantize 1    --in TestImages/ramp.bmp --out OutputImages/ramp_quant_1.bmp
./Assignment1   --quantize 2    --in TestImages/ramp.bmp --out OutputImages/ramp_quant_2.bmp
./Assignment1   --quantize 4    --in TestImages/ramp.bmp --out OutputImages/ramp_quant_4.bmp
  
./Assignment1   --rDither 1    --in TestImages/ramp.bmp --out OutputImages/ramp_rDither_1.bmp
./Assignment1   --rDither 2    --in TestImages/ramp.bmp --out OutputImages/ramp_rDither_2.bmp
./Assignment1   --rDither 4    --in TestImages/ramp.bmp --out OutputImages/ramp_rDither_4.bmp
  
./Assignment1   --oDither2x2 1    --in TestImages/ramp.bmp --out OutputImages/ramp_ordDither_1.bmp
./Assignment1   --oDither2x2 2    --in TestImages/ramp.bmp --out OutputImages/ramp_ordDither_2.bmp
./Assignment1   --oDither2x2 4    --in TestImages/ramp.bmp --out OutputImages/ramp_ordDither_4.bmp
   
./Assignment1   --fsDither 1    --in TestImages/ramp.bmp --out OutputImages/ramp_fsDith_1.bmp
./Assignment1   --fsDither 2    --in TestImages/ramp.bmp --out OutputImages/ramp_fsDith_2.bmp
./Assignment1   --fsDither 4    --in TestImages/ramp.bmp --out OutputImages/ramp_fsDith_4.bmp

./Assignment1   --blur3x3   --in TestImages/sully.bmp --out OutputImages/sully_blur.bmp
 
./Assignment1   --edges3x3  --in TestImages/sully.bmp --out OutputImages/sully_edge.bmp

./Assignment1   --scaleNearest .7   --in TestImages/stripe.2.bmp --out OutputImages/stripe_scaleNear_0.7.bmp
./Assignment1   --scaleNearest 1    --in TestImages/stripe.2.bmp --out OutputImages/stripe_scaleNear_1.bmp
./Assignment1   --scaleNearest 1.3  --in TestImages/stripe.2.bmp --out OutputImages/stripe_scaleNear_1.3.bmp
 
./Assignment1   --scaleBilinear .7   --in TestImages/stripe.2.bmp --out OutputImages/stripe_scaleBilinear_0.7.bmp
./Assignment1   --scaleBilinear 1    --in TestImages/stripe.2.bmp --out OutputImages/stripe_scaleBilinear_1.bmp
./Assignment1   --scaleBilinear 1.3  --in TestImages/stripe.2.bmp --out OutputImages/stripe_scaleBilinear_1.3.bmp

./Assignment1   --scaleGaussian .7   --in TestImages/stripe.2.bmp --out OutputImages/stripe_scaleGaussian_0.7.bmp
./Assignment1   --scaleGaussian 1    --in TestImages/stripe.2.bmp --out OutputImages/stripe_scaleGaussian_1.bmp
./Assignment1   --scaleGaussian 1.3  --in TestImages/stripe.2.bmp --out OutputImages/stripe_scaleGaussian_1.3.bmp

./Assignment1   --rotateNearest 30  --in TestImages/stripe.2.bmp --out OutputImages/stripe_rotNear_30.bmp
./Assignment1   --rotateBilinear 30 --in TestImages/stripe.2.bmp --out OutputImages/stripe_rotBi_30.bmp
./Assignment1   --rotateGaussian 30 --in TestImages/stripe.2.bmp --out OutputImages/stripe_rotGauss_30.bmp