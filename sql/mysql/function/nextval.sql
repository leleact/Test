@delimiter %%%;
CREATE FUNCTION nextval (seq_name VARCHAR(100)) RETURNS bigint(20) NOT DETERMINISTIC MODIFIES SQL DATA
BEGIN
    DECLARE cur_val bigint(20);
    SELECT
    sequence_cur_value INTO cur_val
    FROM
    sequence.sequence_data
    WHERE
    sequence_name = seq_name;

    IF cur_val IS NOT NULL THEN
        UPDATE
        rundb.sequence_data
        SET
        sequence_cur_value = IF (
            (sequence_cur_value + sequence_increment) > sequence_max_value,
            IF (
                sequence_cycle = TRUE,
                sequence_min_value,
                NULL
                ),
                sequence_cur_value + sequence_increment
                )
        WHERE
        sequence_name = seq_name
        ;
      END IF;
      RETURN cur_val;
END
%%%
@delimiter ;
%%%
