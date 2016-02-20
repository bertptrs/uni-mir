#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include "cv.h"
#include "sift.h"
#include "imgfeatures.h"
#include "kdtree.h"
#include "xform.h"
#include "utils.h"

using namespace std;

/* the maximum number of keypoint NN candidates to check during BBF search */
#define KDTREE_BBF_MAX_NN_CHKS 200

/* threshold on squared ratio of distances between NN and 2nd NN */
#define NN_SQ_DIST_RATIO_THR 0.5

struct featureset {
	struct feature* features;
	int nFeatures;
};

int similarity(featureset a, featureset b)
{
	int similarPoints = 0;

	// Mostly taken from test.cpp main code.
	kd_node* kd_root = kdtree_build(b.features, b.nFeatures);
	for (int i = 0; i < a.nFeatures; i++) {
		feature* feat = a.features + i;
		feature** nbrs;
		int k = kdtree_bbf_knn(kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS);

		if (k == 2) {
			double d0, d1;
			d0 = descr_dist_sq( feat, nbrs[0] );
			d1 = descr_dist_sq( feat, nbrs[1] );
			if( d0 < d1 * NN_SQ_DIST_RATIO_THR ) {
				similarPoints++;
			}
		}

		free(nbrs);
	}

	kdtree_release(kd_root);
	return similarPoints;
}

int main(int argc, char* argv[])
{
	if (argc != 4) {
		cerr << "Usage: " << argv[0] << " [image1.jpg] [image2.jpg] [image3.jpg] " << endl;
		return 1;
	}

	IplImage* images[3];
	featureset features[3];
	int similarities[2];

	// Read in images and determine points.
	for (int i = 0; i < 3; i++) {
		images[i] = read_jpeg_file(argv[i + 1]);
		features[i].nFeatures = sift_features(images[i], &features[i].features);
		cout << "Read " << features[i].nFeatures << " for " << argv[i + 1] << endl;
	}

	// Compare the images
	for (int i = 0; i < 2; i++) {
		similarities[i] = similarity(features[0], features[i + 1]);
	}

	cout << argv[1] << " is most similar to ";
	if (similarities[0] > similarities[1]) {
		cout << argv[2];
	} else {
		cout << argv[3];
	}
	cout << " on " << max(similarities[0], similarities[1]) << " points." << endl;

	// Cleanup data afterwards
	for (int i = 0; i < 3; i++) {
		cvReleaseImage(&images[i]);
		free(features[i].features);
	}


	return 0;
}
