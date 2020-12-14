echo building using -O2
make clean all EXTRACXXFLAGS="-O2"

COMMAND_PREFIX="./gzipUtil.out inflate"

function runAndCompareOutput() {
    local src=$1
    local dest=$1
    echo '********************'
    echo running the following command
    echo "$COMMAND_PREFIX $src $dest.out1"
    $COMMAND_PREFIX $src $dest.out1 &>> /dev/null

    echo '********************'

    echo running native gzip util now
    echo "gzip -ndc $src > $dest.out2"
    gzip -ndc $src > $dest.out2

    echo '********************'

    echo running diff command to compare output of both commands
    echo "diff <(xxd $dest.out1) <(xxd $dest.out2)"
    if diff <(xxd $dest.out1) <(xxd $dest.out2); then
        echo files match
    else
        echo files do not match
    fi

    echo '********************'
}

for archive in ./samples/*.gz; do
    runAndCompareOutput "$archive"
done

