1.During driver update, execute ./amlogic_porting_process.sh to regenerate BUILD.bazel.

2.Executing ./amlogic_porting_process.sh will additionally automatically apply the patches in the porting_patches directory.
  If you do not wish to automatically apply the patches, please execute the command with args: ./amlogic_porting_process.sh --skip-s1

3.Porting Patches Description:
porting_patches/amlogic_porting:    Amlogic porting patches based on trunk SDK + reference hardware board environment
porting_patches/wait_realtek_merge: !!!Patches to be evaluated and backported by Realtek!!!
