<p align="center">
  <a href="https://github.com/othneildrew/Best-README-Template">
    <img src=".github/images/pidroid-icon.png" alt="Logo" width="240" height="240">
  </a>

  <h1 align="center">Pidroid</h1>

</p>

<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Motivation](#motivation)
  * [Built With](#built-with)
  * [Roadmap](#roadmap)
* [Usage](#usage)
	* [Scripts](#scripts)
	* [Docker usage](#docker-usage)
	* [Local usage](#local-usage)
	* [Visual Studio Code usage](#vscode-usage)
* [Project branch management](#pbmanagment)
	* [Main branch](#main-branch)
	* [Secondary branch](#secondary-branch)
	* [Others](#othersbranch)

## About the Project

Pidroid is android library to perform fast face detection, pupil detection and landmarks detection without third-party dependencies dependencies.

### Motivation

This library has been developed to facilitate the integration of a face detection method in your Android application.

Usually, to install a face detection library, some integration with third-party software such as OpenCV or Tensorflow Lite or registration on a platform such as Firebase is necessary.

On the other hand, the weight of face detection models is usually high, which causes the application to grow unnecessarily.

Pidroid provides a completely dependency-free alternative to make it easier for developers to use these libraries.

### Built With

This library currently is full developed in Kotlin with C++, no additional dependencies or installation is needed :) 


### Roadmap

- [x] High speed processing
- [x] Does not require additional dependencies
- [x] There is no need for image preprocessing prior detection
- [x] Face detection
- [x] Pupil detection
- [x] Landmarks detection
- [ ] Publish Library as Github Package
- [ ] Pure Java Pidroid (to delete avoid Kotlin dependency)
- [ ] Rotated faces detection
- [ ] Neon support
- [ ] SSE support
- [ ] Tegra support
