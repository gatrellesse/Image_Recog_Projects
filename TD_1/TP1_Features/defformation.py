import cv2
import numpy as np
import matplotlib.pyplot as plt


def transform_image(img, angle_deg=45, tx=5, ty=30):
    h, w = img.shape[:2]
    M = cv2.getRotationMatrix2D((w / 2, h / 2), angle_deg, 1.0)
    M[0, 2] += tx
    M[1, 2] += ty
    img_transformed = cv2.warpAffine(img, M, (w, h))
    return img_transformed, M


def detect_features(img_gray, method='orb'):
    if method == 'orb':
        detector = cv2.ORB_create(nfeatures=500)
    elif method == 'kaze':
        detector = cv2.KAZE_create()
    else:
        raise ValueError("Unknown detector")
    keypoints, descriptors = detector.detectAndCompute(img_gray, None)
    return keypoints, descriptors, detector


def match_features(desc1, desc2, method='cross', detector='orb'):
    if detector == 'orb':
        norm_type = cv2.NORM_HAMMING
    else:
        norm_type = cv2.NORM_L2
    
    bf = cv2.BFMatcher(norm_type)

    if method == 'cross':
        matches = bf.match(desc1, desc2)
    elif method == 'ratio':
        raw_matches = bf.knnMatch(desc1, desc2, k=2)
        matches = [m for m, n in raw_matches if m.distance < 0.8 * n.distance]
    else:
        raise ValueError("Unknown matching method")

    matches = sorted(matches, key=lambda x: x.distance)
    return matches


def evaluate_matching(kp1, kp2, matches, transformation_matrix, threshold=3.0):
    correct = 0
    distances = []

    for m in matches:
        pt1 = np.array([*kp1[m.queryIdx].pt, 1.0])
        pt2 = np.array(kp2[m.trainIdx].pt)
        pt1_transformed = transformation_matrix @ pt1
        dist = np.linalg.norm(pt2 - pt1_transformed[:2])
        distances.append(dist)
        if dist < threshold:
            correct += 1

    inlier_rate = correct / len(matches) if matches else 0
    mean_error = np.mean(distances) if distances else float('inf')

    return inlier_rate, mean_error, correct


def draw_keypoints(img, keypoints, title="Keypoints"):
    img_kp = cv2.drawKeypoints(img, keypoints, None, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    plt.figure(figsize=(8, 6))
    plt.title(title)
    plt.imshow(img_kp, cmap='gray')
    plt.axis('off')
    plt.show()

def draw_matches(img1, kp1, img2, kp2, matches, max_matches=30, title="Matches"):
    img_match = cv2.drawMatches(img1, kp1, img2, kp2, matches[:max_matches], None, flags=2)
    plt.figure(figsize=(14, 6))
    plt.title(title)
    plt.imshow(img_match)
    plt.axis('off')
    plt.show()

# ----------------------
# Main pipeline
# ----------------------
def feature_matching_pipeline(image_path, detector_type='orb', matcher_type='cross'):
    img = cv2.imread(image_path)
    if img is None:
        raise FileNotFoundError("Image not found:", image_path)

    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img_transformed, transform_matrix = transform_image(img)

    img_transformed_gray = cv2.cvtColor(img_transformed, cv2.COLOR_BGR2GRAY)

    kp1, desc1, _ = detect_features(img_gray, detector_type)
    kp2, desc2, _ = detect_features(img_transformed_gray, detector_type)

    matches = match_features(desc1, desc2, method=matcher_type, detector=detector_type)

    inlier_rate, mean_error, correct = evaluate_matching(kp1, kp2, matches, transform_matrix)

    print(f"\n--- {detector_type.upper()} + {matcher_type.upper()} ---")
    print(f"Total matches: {len(matches)}")
    print(f"Correct matches (dist < 3px): {correct}")
    print(f"Inlier rate: {inlier_rate:.2%}")
    print(f"Mean error: {mean_error:.2f} px")

    # Visualize results
    draw_keypoints(img_gray, kp1, "Keypoints in Original Image")
    draw_keypoints(img_transformed_gray, kp2, "Keypoints in Transformed Image")
    draw_matches(img_gray, kp1, img_transformed_gray, kp2, matches, title="Feature Matches")

if __name__ == "__main__":
    feature_matching_pipeline(
        image_path= "Image_Pairs\\torb_small1.png",
        detector_type='kaze',    # or 'orb'
        matcher_type='ratio'     # or 'cross'
    )
