
var db = require('./models/db');
var cp = require('child_process');

SLEEP_IN_MS = 1000;

function compile(submit) {
  if (submit.lang === 'C++') {
    console.log('id:' + submit._id);
    cp.exec('g++ -Wall -static -O3 ./uploads/' + submit._id +
      '.cpp -o ./exe/' + submit._id + '.exe',
      function(err, stdout, stderr) {
        if (err) {
          console.log('id:' + submit._id + ' error: ' + err);
          db.submits.update({_id: submit._id},
              {$set: {status: 1}},
              function(err) {
                if (err) console.log('Update failed: ' + err);
              });
        } else {
          console.log('id:' + submit._id + ' successful');
          db.submits.update({_id: submit._id},
              {$set: {status: 2}},
              function(err) {
                if (err) console.log('Update failed: ' + err);
              });
        }
        // console.log('stdout: ' + stdout);
        // console.log('stderr: ' + stderr);
        // update database
      });
  } else {
    console.log("I cannot compile java now.");
  }
}

function pick_submit() {
  db.submits.findOne({status: 0}, function(err, submit) {
    if (err || !submit) {
      console.log("Not found uncompiled code.");
      // sleep here?
      return;
    }
    compile(submit);
  });
}

console.log('Compiler is running.');
setInterval(pick_submit, SLEEP_IN_MS);
