# SPDX-License-Identifier: Apache-2.0
#
# Copyright (C) 2025 Amlogic Inc. All rights reserved.
#
#!/bin/bash
#
# Created by bo.li on 2025/12/26
#

readonly CUR_PATH="$(readlink -f $(dirname $0))"
readonly DRIVER_DIR="rtl88x2CS"
readonly ARGS=$@

#step 1. apply patches
function apply_patches()
{
 (
  echo
  echo "###### apply patches ######"
  cd $CUR_PATH
  patch_file=""
  for patch_file in `find porting_patches/ -name "*.patch"`; do
   patch_basename=`basename $patch_file`
   DRY_RUN_OUTPUT=$(patch -p1 -i "$patch_file" --forward --dry-run 2>&1)
   if [ $? -eq 0 ]; then
    echo
    echo "#Starting to apply patch [$patch_basename]..."
    patch -p1 -i "$patch_file" --forward
   else
    if echo "$DRY_RUN_OUTPUT" | grep -q "already applied" || echo "$DRY_RUN_OUTPUT" | grep -q "Reversed (or previously applied) patch detected"; then
     echo
     echo "#Patch [$patch_basename] has already been applied, no need to reapply."
    else
     echo
     echo "#Patch [$patch_basename] is invalid, failed to apply patch: $DRY_RUN_OUTPUT"
    fi
   fi
  done
  echo
 )
}

#step 2. generate BUILD.bazel
function generate_build_bazel()
{
 (
  echo "###### generate BUILD.bazel ######"
  cd $CUR_PATH/$DRIVER_DIR
  make CONFIG_PLATFORM_I386_PC=n CONFIG_PLATFORM_AML_S905=y CONFIG_GKI_BAZEL=y KERNEL_BUILD="//common_drivers:amlogic" generate_build_bazel
 )
}

if [[ x$(echo -n "$ARGS" | grep -w "\--skip-s1" 2>/dev/null) == x ]]; then
 apply_patches
fi

if [[ x$(echo -n "$ARGS" | grep -w "\--skip-s2" 2>/dev/null) == x ]]; then
 generate_build_bazel
fi
