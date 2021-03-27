var debug = require('bindings')('debug');
console.time("c");
for (let index = 0; index < 1000; index++) {
    
    // console.log(index.toString());
    debug.log(index.toString());
}
// addon.Test("1");
// addon.Test("2");
console.timeEnd("c");
