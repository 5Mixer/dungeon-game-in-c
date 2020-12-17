let project = new Project('Project');

project.addFile('Sources/**');
project.setDebugDir('Deployment');

resolve(project);
