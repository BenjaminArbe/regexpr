echo "Running unit tests:"

for i in test/*_t
do
    $i 2>&1 >> test/test.log
    if [ ! $? ]
    then
        echo "ERROR in test $i: here's test/test.log"
        echo "------"
        tail test/test.log
        exit 1
    fi
done

echo ""