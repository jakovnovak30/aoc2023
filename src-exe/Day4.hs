module Day4 where

data Card = Card Int ([Int], [Int])

getCardId :: Card -> Int
getCardId (Card i _) = i

solution1 :: [String] -> Int
solution1 = sum . map (checkCard . getNumbers)

getNumbers :: String -> ([Int], [Int])
getNumbers str = (first str, second str)
  where
    first = map read . words . takeWhile (/= '|') . drop 1 . dropWhile (/= ':')
    second = map read . drop 1 . words . dropWhile (/= '|') . drop 1 . dropWhile (/= ':')

getCard :: String -> Card
getCard str = Card (card_id str) (first str, second str)
  where
    card_id = read . last . words . takeWhile (/= ':')
    first = map read . words . takeWhile (/= '|') . drop 1 . dropWhile (/= ':')
    second = map read . drop 1 . words . dropWhile (/= '|') . drop 1 . dropWhile (/= ':')

checkCard :: ([Int], [Int]) -> Int
checkCard (correct, tried) = foldr (\x acc -> if x `elem` correct then increment acc else acc) 0 tried
  where
    increment x
      | x == 0 = 1
      | otherwise  = x*2

solution2 :: [String] -> Int
solution2 = (`div` 2) . (\cards -> checkCards 0 cards cards) . map getCard

checkCards :: Int -> [Card] -> [Card] -> Int
checkCards acc [] _ = acc
checkCards acc (c : cs) org_cs
  | matches == 0 = 1 + checkCards (acc + 1) cs org_cs
  | otherwise = 1 + checkCards (acc + 1) (getCopies matches (getCardId c) org_cs ++ cs) org_cs
  where
    matches = getMatches c

getCopies :: Int -> Int -> [Card] -> [Card]
getCopies n after cs = take n $ drop after cs

getMatches :: Card -> Int
getMatches (Card _ (correct, tried)) = foldr (\x acc -> if x `elem` correct then acc+1 else acc) 0 tried
