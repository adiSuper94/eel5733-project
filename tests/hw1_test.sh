#!bin/sh
build_dir=$(pwd)/build
tests_dir=$(pwd)/tests
if [ ! -d "$build_dir" ]; then
  echo "Error: build dir not found at : $build_dir"
  echo "Tip: make sure you are in the root/base dir of the project"
  exit 1
fi
if [ ! -d "$tests_dir" ]; then
  echo "Error: test dir not found at: $tests_dir"
  echo "Tip: make sure you are in the root/base dir of the project"
  exit 1
fi

ef_bin=${build_dir}/email_filter
cf_bin=${build_dir}/calendar_filter
lu_bin=${build_dir}/location_updater

if [ ! -e "$ef_bin" ] || [ ! -e "$cf_bin" ] || [ ! -e "$lu_bin" ]; then
  echo "Error: build dir: $build_dir"
  echo "Error: Some or all of the following binaries not found in build_dir : [email_filter, calendar_filter, location_updater]\n"
  echo "Tip: make sure you have compiled the source"
  exit 1
fi

cf_test_inputs_dir=$tests_dir/inputs/hw1/calendar-filter
lu_test_inputs_dir=$tests_dir/inputs/hw1/email-filter
cf_test_inputs=$(ls "$tests_dir/inputs/hw1/calendar-filter")
lu_test_inputs=$(ls "$tests_dir/inputs/hw1/email-filter")

if [ ! -e "$cf_test_inputs_dir" ] || [ ! -e "$lu_test_inputs_dir" ]; then
  echo "Error: Sorry, you might have to test manually! Or Debug this :("
  exit 1
fi

cf_output_dir=$tests_dir/outputs/hw1/calendar-filter
ef_output_dir=$tests_dir/outputs/hw1/email-filter
if [ ! -e "$cf_output_dir" ] || [ ! -e "$ef_output_dir" ]; then
  echo "Error: Output dirs not created. Sorry, you might have to test manually! Or Debug this :("
  exit 1
fi
rm -rf $cf_output_dir
rm -rf $ef_output_dir
mkdir -p $cf_output_dir
mkdir -p $ef_output_dir

echo "=====STARTING TESTS=====\n"

cd $build_dir
echo "RUNNING calendar-filter TESTS IN : $cf_test_inputs_dir"
for i_f in $cf_test_inputs_dir/*.txt; do
  o_f=$cf_output_dir/$(basename "$i_f" | sed s/input/output/)
  ($cf_bin < "$i_f") > $o_f
done
echo "OUTPUT GENERATED in $cf_output_dir\n"

echo "RUNNING location_updater TESTS IN : $lu_test_inputs_dir"
for i_f in $lu_test_inputs_dir/*.txt; do
  o_f=$ef_output_dir/$(basename "$i_f" | sed s/input/output/)
  ($lu_bin < "$i_f") > $o_f
done
echo "OUTPUT GENERATED in $ef_output_dir\n"


echo "==========END==========="
exit 0
